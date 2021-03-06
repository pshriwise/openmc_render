
#include "openmc_scenes.h"
#include "openmc/capi.h"


std::vector<std::shared_ptr<Material>> materials;

struct HitSpan {

  // Intersection
  HitSpan& operator &=(const HitSpan& other) {
    if (other.min > min) {
      min = other.min;
      min_surf = other.min_surf;
    }
    max = std::min(max, other.max);
    return *this;
  }

  HitSpan operator &(const HitSpan& other) {
    HitSpan result = *this;
    return result &= other;
  }

  // Union
  HitSpan& operator |=(const HitSpan& other) {
    if (other.min < min) {
      min = other.min;
      min_surf = other.min_surf;
    }
    max = std::max(max, other.max);
    return *this;
  }

  HitSpan operator |(const HitSpan& other) {
    HitSpan result = *this;
    return result |= other;
  }

  bool is_valid() { return min <= max; }

  double min;
  double max;
  int32_t min_surf;
};

bool OpenMCCell::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
  // check for a hit
  openmc::Position p = r.orig.e;
  openmc::Direction u = r.dir.e;
  u /= u.norm();

  const auto& rpn = cell_ptr_->rpn_;

  // compute the minimum of spans for all surfaces in the cell
  std::vector<double> t_minimums(rpn.size());
  std::vector<HitSpan> spans(rpn.size());

  for (int32_t i = 0; i < rpn.size(); ++i) {
    const auto& token = rpn[i];
    auto& this_span = spans[i];

    if (token >= openmc::OP_UNION) {
      continue;
    }

    const auto& surf = openmc::model::surfaces[std::abs(token) -1];

    this_span.min_surf = token;
    // if we're inside the halfspace, minimum intersection is zero
    if (token * surf->evaluate(r.orig.e) > 0) {
      this_span.min = 0.0;
      // check for exiting intersection
      this_span.max = surf->distance(p, u, false);
    } else {
      // attempt to find an intersection with the halfspace
      this_span.min = surf->distance(p, u, false);
      if (this_span.min < openmc::INFTY) {
        this_span.max = this_span.min + surf->distance(p + u * (this_span.min + 1E-06), u, false);
      } else {
        this_span.max = -INFTY;
      }
    }
  }

  HitSpan final_span = {-INFTY, INFTY};

  if (cell_ptr_->simple_) {
    for (int i = 0; i < rpn.size(); ++i) {
      const auto& token = rpn[i];
      final_span &= spans[i];
    }
  } else {
    std::vector<HitSpan> stack(rpn.size());
    int i_stack = -1;
    for (int i = 0; i < rpn.size(); ++i) {
      auto token = rpn[i];
      if (token == openmc::OP_UNION) {
        stack[i_stack - 1] = stack[i_stack - 1] | stack[i_stack];
        i_stack--;
      } else if (token == openmc::OP_INTERSECTION) {
        stack[i_stack - 1] = stack[i_stack - 1] & stack[i_stack];
        i_stack--;
      } else {
        i_stack++;
        stack[i_stack] = spans[i];
      }
    }
    final_span = stack.front();
  }

  // if the span is invlaid, there is no hit
  if (!final_span.is_valid()) { return false; }
  // if the final span is valid, continue
  double t = final_span.min;
  int32_t surf_idx = final_span.min_surf;

  if (t > t_min && t < t_max && t < openmc::INFTY) {
    rec.t_ = t;
    rec.p_ = r.at(t);

    auto openmc_norm = openmc::model::surfaces[std::abs(surf_idx) - 1]->normal(rec.p_.e);
    if (surf_idx < 0) openmc_norm *= -1;
    Vec3 outward_normal{openmc_norm.x, openmc_norm.y, openmc_norm.z};
    rec.set_face_normal(r, unit_vector(outward_normal));
    rec.material_ = material_;
    return true;
  }

  return false;
}

bool OpenMCSphere::hit (const Ray& r, double t_min, double t_max, Hit& rec) const {

  // check for a hit
  openmc::Position p = r.orig.e;
  openmc::Direction u = r.dir.e;
  u /= u.norm();
  double dist = openmc_sphere_->distance(p, u, false);

  if (dist < openmc::INFTY) {
    // compute t-value
    rec.t_ = dist;
    rec.p_ = r.at(dist);
    auto openmc_norm = openmc_sphere_->normal(rec.p_.e);
    Vec3 outward_normal{openmc_norm.x, openmc_norm.y, openmc_norm.z};
    rec.set_face_normal(r, unit_vector(outward_normal));
    rec.material_ = material_;
    return true;
  }

  return false;
}

Scene openmc_spheres() {

  ObjectList objects;

  auto material1 = std::make_shared<Lambertian>(Color(0.0, 0.0, 0.5));
  auto material2 = std::make_shared<Lambertian>(Color(0.5 , 0.5, 0.5));

  objects.add(std::make_shared<OpenMCSphere>(Point3(0, 0, -1), 0.5, material1));
  objects.add(std::make_shared<OpenMCSphere>(Point3(0, -100.5, -1), 100, material2));

  // Setup camera
  Point3 camera_position{0, 5, 0};
  Point3 camera_target{0, 0, -1};
  double field_of_view = 90;
  double aperture = 2.0;

  Camera camera(camera_position,
                camera_target,
                Point3(0, 1, 0),
                field_of_view,
                ASPECT_RATIO,
                aperture,
                (camera_position - camera_target).length());

  return {objects, camera};
}

Scene openmc_setup() {
  openmc_init(0, nullptr, nullptr);

  ObjectList objects;

  // create a couple of materials
  materials.push_back(std::make_shared<Metal>(Color(0.5, 0.5, 0.5), 0.0));
  materials.push_back(std::make_shared<Lambertian>(Color(0.0, 0.2, 0.8)));

  // add more materials as needed
  while(materials.size() < openmc::model::cells.size()) {
    materials.push_back(std::make_shared<Lambertian>(Color::random()));
  }

  // create an object for each cell in the OpenMC problem

  for (int i = 0; i < openmc::model::cells.size(); ++i) {
    // randomize material
    const auto& cell = openmc::model::cells[i];

    auto object = std::make_shared<OpenMCCell>(cell.get(), materials[i]);

    objects.add(object);
  }

  // Setup camera
  Point3 camera_position{0, 0.25, 1};
  Point3 camera_target{0, 0, -1};
  double field_of_view = 90;
  double aperture = 0.0;

  Camera camera(camera_position,
                camera_target,
                Point3(0, 1, 0),
                field_of_view,
                ASPECT_RATIO,
                aperture,
                (camera_position - camera_target).length());

  return {objects, camera};
}
