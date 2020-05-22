
#ifndef CAMERA_H
#define CAMERA

#include "constants.h"
#include "ray.h"
#include "vec3.h"

class Camera {
public:

  // Constructor
  Camera (Point3 look_from, Point3 look_at, Vec3 vup, double vert_field_of_view, double aspect_ratio) {

    double theta = deg_to_rad(vert_field_of_view);
    double h = tan(theta / 2.0);
    double viewport_height = 2.0 * h;
    double viewport_width = ASPECT_RATIO * viewport_height;
    double focal_length = 1.0;

    Vec3 w = unit_vector(look_from - look_at);
    Vec3 u = unit_vector(cross(vup, w));
    Vec3 v = cross(w, u);

    origin_ = look_from;
    horizontal_ = viewport_width * u;
    vertical_ = viewport_height * v;
    llc_ = origin_ - horizontal_ / 2.0 - vertical_ / 2.0 - w;
  }

  Ray get_ray(double u, double v) const {
    return Ray(origin_, llc_ + u*horizontal_ + v*vertical_ - origin_);
  }

  // Data Members
  Point3 origin_;
  Point3 llc_;
  Vec3 horizontal_;
  Vec3 vertical_;

};


#endif