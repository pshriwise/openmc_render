
#include "openmc_scenes.h"

bool OpenMCCell::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
  // check for a hit
  openmc::Position p = r.orig.e;
  openmc::Direction u = r.dir.e;
  u /= u.norm();
  auto hit = cell_ptr_->distance(p, u, false);

  if (hit.first < openmc::INFTY) {
  rec.t_ = hit.first;
  rec.p_ = r.at(hit.first);
  auto openmc_norm = openmc::model::surfaces[hit.second]->normal(rec.p_.e);
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
  Point3 camera_position{0, 0, 0};
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