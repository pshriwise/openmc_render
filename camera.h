
#ifndef CAMERA_H
#define CAMERA

#include "constants.h"
#include "ray.h"
#include "vec3.h"

class Camera {
public:

  // Constructor
  Camera (double vert_field_of_view, double aspect_ratio) {
    double theta = deg_to_rad(vert_field_of_view);
    double h = tan(theta / 2.0);
    double viewport_height = 2.0 * h;
    double viewport_width = ASPECT_RATIO * viewport_height;
    double focal_length = 1.0;

    origin_ = Point3(0.0, 0.0, 0.0);
    horizontal_ = Vec3(viewport_width, 0.0, 0.0);
    vertical_ = Vec3(0.0, viewport_height, 0.0);
    llc_ = origin_ - horizontal_ / 2.0 - vertical_ / 2.0 - Vec3(0.0, 0.0, focal_length);
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