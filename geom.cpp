
#include "geom.h"

bool hit_sphere(const Point3& center, double radius, const Ray& r, double& t) {
  Vec3 oc = r.origin() - center;
  double a = dot(r.direction(), r.direction());
  double b = 2.0 * dot(oc, r.direction());
  double c = dot(oc, oc) - radius * radius;
  double discriminant = b*b - 4.0*a*c;
  bool hit = discriminant > 0.0;

  if (hit) {
    t = (-b - sqrt(discriminant)) / (2.0 * a);
  }
  return hit;
}
