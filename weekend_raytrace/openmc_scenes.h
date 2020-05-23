

#ifndef OSCENES_H
#define OSCENES_H

#include "openmc/surface.h"
#include "openmc/cell.h"

#include "scene.h"

Scene openmc_spheres();

// generalized OpenMC CSG class
class OpenMCCell : public Object {
public:
  OpenMCCell(openmc::Cell* cell_ptr, std::shared_ptr<Material> material)
  : cell_ptr_(cell_ptr), material_(material) {};

  // Methods
  virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const;

  std::shared_ptr<Material> material_;
  openmc::Cell* cell_ptr_;
};

class OpenMCSphere : public Object {
public:
  OpenMCSphere(Point3 center, double radius, std::shared_ptr<Material> material)
    : material_(material) {

      // create an OpenMC object for the sphere

      openmc_sphere_ = std::make_shared<openmc::SurfaceSphere>();
      openmc_sphere_->x0_ = center.x();
      openmc_sphere_->y0_ = center.y();
      openmc_sphere_->z0_ = center.z();
      openmc_sphere_->radius_ = radius;

  };

  // Methods
  virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const;

  std::shared_ptr<Material> material_;
  std::shared_ptr<openmc::SurfaceSphere> openmc_sphere_;
};


#endif