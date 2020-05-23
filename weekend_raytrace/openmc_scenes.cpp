

#include "scene.h"
#include "geom.h"

#include "openmc/cell.h"
#include "openmc/surface.h"

// generalized OpenMC CSG class
class OpenMCCell : public Object {
public:
  OpenMCCell(openmc::CSGCell* cell_ptr) : cell_ptr_(cell_ptr) {};

  // Methods
  virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const;

  std::shared_ptr<Material> material_;
  openmc::CSGCell* cell_ptr_;
};

bool OpenMCCell::hit(const Ray& r, double t_min, double t_max, Hit& rec) const {
  // check for a hit
  openmc::Position p = r.orig.e;
  openmc::Direction u = r.dir.e;
  u /= u.norm();
  auto hit = cell_ptr_->distance(p, u, false);

  if (hit.first < openmc::INFTY) {
  rec.t_ = hit.first;
  rec.p_ = r.at(hit.first);
  auto openmc_norm = openmc::surfaces[hit.second]->normal(rec.p_.e);
  Vec3 outward_normal{openmc_norm.x, openmc_norm.y, openmc_norm.z};
  rec.set_face_normal(r, unit_vector(outward_normal));
  rec.material_ = material_;
  return true;
  }

  return false;
}

Scene openmc_cylinder() {

}