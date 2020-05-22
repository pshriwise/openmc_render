
#include <iostream>
#include <memory>

#include "camera.h"
#include "color.h"
#include "constants.h"
#include "geom.h"
#include "rand.h"
#include "ray.h"

Color ray_color(const Ray& r, const ObjectList& scene, int depth=0) {
  Hit hit;
  if (depth > MAX_BOUNCE) {
    return Color(0,0,0);
  }

  if (scene.hit(r, 0.001, INFTY, hit)) {
      // compute the location of the hit
      auto& normal = hit.n_;
      Point3 target = hit.p_ + hit.n_ + random_in_hemisphere(hit.n_);

      const double bounce_reduction = 0.5;
      return bounce_reduction * ray_color(Ray(hit.p_, target - hit.p_), scene, depth++);
  }

  // Color using background
  Vec3 unit_direction = unit_vector(r.direction());
  double t = 0.5*(unit_direction.y() + 1.0);
  return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
}

int main() {

  const int image_width = 384;
  const int image_height = static_cast<int>(image_width / ASPECT_RATIO);
  std::cout << "P3\n" << image_width << " " << image_height << "\n" << IRGB_MAX << "\n";

  // Rendering objects
  ObjectList scene;
  scene.add(std::make_shared<Sphere>(Point3(0, 0, -1.0), 0.5));
  scene.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

  Camera camera;

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
    for (int i = 0; i < image_width; ++i) {
      Color pixel_color{0.0, 0.0, 0.0};
      for (int s = 0; s < SAMPLES_PER_PIXEL; ++s) {
        double u = (i + nrand()) / (image_width - 1);
        double v = (j + nrand()) / (image_height - 1);
        Ray r = camera.get_ray(u, v);
        pixel_color += ray_color(r, scene);
      }
      write_color(std::cout, pixel_color, SAMPLES_PER_PIXEL);
    }
  }
  std::cerr << "\nDone\n";
  return 0;
}