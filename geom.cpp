
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
      rec.n_ = (rec.p_ - center_) / radius_;
      return true;
    }
    t = (-half_b + root) / a;
    if (t > t_min && t < t_max) {
      rec.t_ = t;
      rec.p_ = r.at(t);
      rec.n_ = (rec.p_ - center_) / radius_;
      return true;
    }
  }
  return false;
}
