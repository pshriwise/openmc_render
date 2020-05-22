
#ifndef HIT_H
#define HIT_H

#include <memory>

#include "vec3.h"
#include "ray.h"

class Material;

struct Hit {
  Point3 p_;
  Vec3 n_;
  bool front_face_;
  std::shared_ptr<Material> material_;

  inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
    front_face_ = dot(r.direction(), outward_normal) < 0.0;
    n_ = front_face_ ? outward_normal : -outward_normal;
  }

  double t_;
};

#endif