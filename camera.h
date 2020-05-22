
#ifndef CAMERA_H
#define CAMERA

#include "constants.h"
#include "ray.h"
#include "vec3.h"

class Camera {
public:

  // Constructor
  Camera (Point3 look_from,
          Point3 look_at,
          Vec3 vup,
          double vert_field_of_view,
          double aspect_ratio,
          double aperture,
          double focus_dist) {

    double theta = deg_to_rad(vert_field_of_view);
    double h = tan(theta / 2.0);
    double viewport_height = 2.0 * h;
    double viewport_width = ASPECT_RATIO * viewport_height;
    double focal_length = 1.0;

    w_ = unit_vector(look_from - look_at);
    u_ = unit_vector(cross(vup, w_));
    v_ = cross(w_, u_);

    origin_ = look_from;
    horizontal_ = focus_dist * viewport_width * u_;
    vertical_ = focus_dist * viewport_height * v_;
    llc_ = origin_ - horizontal_ / 2.0 - vertical_ / 2.0 - focus_dist * w_;
    lens_radius_ = aperture / 2.0;
  }

  Ray get_ray(double s, double t) const {
    Vec3 rd = lens_radius_ * random_in_unit_disk();
    Vec3 offset = u_ * rd.x() + v_ * rd.y();

    return Ray(origin_ + offset,
               llc_ + s*horizontal_ + t*vertical_ - origin_ - offset);
  }

  // Data Members
  Point3 origin_;
  Point3 llc_;
  Vec3 horizontal_;
  Vec3 vertical_;
  Vec3 u_, v_, w_;
  double lens_radius_;
};


#endif