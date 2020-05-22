


#ifndef GEOM_H
#define GEOM_H

#include <memory>
#include <vector>

#include "hit.h"
#include "material.h"
#include "ray.h"

class Object {
public:
  virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const = 0;
};

class ObjectList : public Object {

public:
  // Constructors
  ObjectList() {};
  ObjectList(std::shared_ptr<Object> object) { add(object); }

  // Methods
  void clear() { objects_.clear(); }
  void add(std::shared_ptr<Object> object) { objects_.push_back(object); }

  virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const;

  // Data Members
  std::vector<std::shared_ptr<Object>> objects_;
};


class Sphere : public Object {
public:
  // Constructors
  Sphere() {};
  Sphere(Point3 center, double radius, std::shared_ptr<Material> material)
  : center_(center), radius_(radius), material_(material) {};

  // Methods
  virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const;

  // Data members
  Point3 center_;
  double radius_;
  std::shared_ptr<Material> material_;
};

bool hit_sphere(const Point3& center, double radius, const Ray& r, double& t);

#endif