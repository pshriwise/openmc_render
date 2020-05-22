
#include <iostream>
#include <fstream>
#include <memory>

#include "camera.h"
#include "color.h"
#include "constants.h"
#include "geom.h"
#include "rand.h"
#include "ray.h"

Color ray_color(const Ray& r, const ObjectList& scene, int depth = 0) {
  Hit hit;
  if (depth > MAX_BOUNCE) {
    return Color(0,0,0);
  }

  if (scene.hit(r, 0.001, INFTY, hit)) {
    Ray scattered;
    Color attenuation;
    if (hit.material_->scatter(r, hit, attenuation, scattered)) {
      return attenuation * ray_color(scattered, scene, ++depth);
    }
    return Color(0.0, 0.0, 0.0);
  }

  // if (scene.hit(r, 0.001, INFTY, hit)) {
  //     // compute the location of the hit
  //     auto& normal = hit.n_;
  //     Point3 target = hit.p_ + hit.n_ + random_in_hemisphere(hit.n_);

  //     const double bounce_reduction = 0.5;
  //     return bounce_reduction * ray_color(Ray(hit.p_, target - hit.p_), scene, ++depth);
  // }

  // Color using background
  Vec3 unit_direction = unit_vector(r.direction());
  double t = 0.5*(unit_direction.y() + 1.0);
  return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
}

int main() {

  std::ofstream outfile("image.ppm");

  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / ASPECT_RATIO);
  outfile << "P3\n" << image_width << " " << image_height << "\n" << IRGB_MAX << "\n";

  // Rendering objects
  ObjectList scene;
  auto material1 = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
  auto material2 = std::make_shared<Lambertian>(Color(0.8 , 0.8, 0.0));

  auto material3 = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.3);
  auto material4 = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 1.0);

  scene.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5, material1));
  scene.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100, material2));
  scene.add(std::make_shared<Sphere>(Point3(1, 0, -1), 0.5, material3));
  scene.add(std::make_shared<Sphere>(Point3(-1, 0, -1), 0.5, material4));


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
      write_color(outfile, pixel_color, SAMPLES_PER_PIXEL);
    }
  }
  std::cerr << "\nDone\n";
  return 0;
}