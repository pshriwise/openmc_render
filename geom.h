


#ifndef GEOM_H
#define GEOM_H

#include "ray.h"

struct Hit {
  Point3 p_;
  Vec3 n_;
  double t_;
};

class Object {
public:
  virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const = 0;
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