

#include <fstream>
#include <memory>
#include <string>

#include "color.h"
#include "openmc_scenes.h"

Color ray_color(const Ray& r, const ObjectList& objects) {
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
      std::cout << "Hit dist: " << hit.t_ << std::endl;
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

int main() {

  Scene scene = openmc_setup();

  Point3 start(0, 5, 0);
  Point3 stop(5.0, 4.0, -1);
  Vec3 dir = unit_vector(stop - start);
  std::cout << "Length: " << (stop-start).length() << std::endl;
  // create a ray
  Ray r(start, dir);

  auto c = ray_color(r, scene.objects());
  std::cout << "Color: " << c[0] <<  " " << c[1] << " " << c[2] << std::endl;
  return 0;
}

