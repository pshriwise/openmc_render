
#include "geom.h"

bool hit_sphere(const Point3& center, double radius, const Ray& r, double& t) {
  Vec3 oc = r.origin() - center;
  double a = r.direction().length_squared();
  double half_b = dot(oc, r.direction());
  double c = oc.length_squared() - radius * radius;
  double discriminant = half_b*half_b - a*c;
  bool hit = discriminant > 0.0;

  if (hit) {
    t = (-half_b - sqrt(discriminant)) / a;
  }

  return hit;
}

bool Sphere::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {

  Vec3 oc = r.origin() - center_;
  double a = r.direction().length_squared();
  double half_b = dot(oc, r.direction());
  double c = oc.length_squared() - radius_ * radius_;
  double discriminant = half_b * half_b - a*c;

  if (discriminant > 0.0) {
    double root = sqrt(discriminant);
    double t = (-half_b - root) / a;
    if (t > t_min && t < t_max) {
      rec.t_ = t;
      rec.p_ = r.at(t);
      auto outward_norm = (rec.p_ - center_) / radius_;
      rec.set_face_normal(r, outward_norm);
      rec.material_ = material_;
      return true;
    }
    t = (-half_b + root) / a;
    if (t > t_min && t < t_max) {
      rec.t_ = t;
      rec.p_ = r.at(t);
      auto outward_norm = (rec.p_ - center_) / radius_;
      rec.set_face_normal(r, outward_norm);
      rec.material_ = material_;
      return true;
    }
  }
  return false;
}

bool ObjectList::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
  Hit temp_hit;
  bool hit_something = false;
  double t_curr = t_max;

  for (const auto& object : objects_) {
    if (object->hit(r, t_min, t_curr, temp_hit) && temp_hit.t_ < t_curr) {
      hit_something = true;
      t_curr = temp_hit.t_;
      rec = temp_hit;
    }
  }
  return hit_something;
}

#ifdef OPENMC
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

// bool OpenMCSphere::hit (const Ray& r, double t_min, double t_max, Hit& rec) const {

//   // check for a hit
//   openmc::Position p = r.orig.e;
//   openmc::Direction u = r.dir.e;
//   u /= u.norm();
//   double dist = openmc_sphere_->distance(p, u, false);

//   if (dist < openmc::INFTY) {
//     // compute t-value
//     rec.t_ = dist;
//     rec.p_ = r.at(dist);
//     auto openmc_norm = openmc_sphere_->normal(rec.p_.e);
//     Vec3 outward_normal{openmc_norm.x, openmc_norm.y, openmc_norm.z};
//     rec.set_face_normal(r, unit_vector(outward_normal));
//     rec.material_ = material_;
//     return true;
//   }

//   return false;
// }
#endif //OPENMC