
#include "trace.h"

Color trace_color(const Ray& r, const ObjectList& objects) {
      // copy of the input ray
  Ray ray = r;
  Color ray_color;
  Hit hit;

  std::vector<Color> stack;
  stack.reserve(MAX_BOUNCE + 1);

  while(true) {

    if (stack.size() > MAX_BOUNCE) {
      return ABSORBED;
    }

    if (objects.hit(ray, 0.001, INFTY, hit)) {
      // scatter, add color contribution and continue to follow the ray
      if (hit.material_->scatter(ray, hit, ray_color, ray)) {
        stack.push_back(ray_color);
        continue;
      // absorption, add null color to the end of the stack
      } else {
        return ABSORBED;
      }
    }

    // ray reached the background, compute background value and add it to the stack
    Vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5*(unit_direction.y() + 1.0);
    stack.push_back((1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0));
    break;
  }

  if (stack.size() > 0) {
    return std::accumulate(stack.begin(), stack.end(), WHITE, std::multiplies<Color>());
  }

  return BLACK;
}