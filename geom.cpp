
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
