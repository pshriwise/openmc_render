


#ifndef GEOM_H
#define GEOM_H

#include <memory>
#include <vector>

#include "ray.h"

struct Hit {
  Point3 p_;
  Vec3 n_;
  bool front_face_;

  inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
    front_face_ = dot(r.direction(), outward_normal) < 0.0;
    n_ = front_face_ ? outward_normal : -outward_normal;
  }

  double t_;
};

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
  Sphere(Point3 center, double radius) : center_(center), radius_(radius) {};

  // Methods
  virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const;

  // Data members
  Point3 center_;
  double radius_;
};

bool hit_sphere(const Point3& center, double radius, const Ray& r, double& t);

#endif