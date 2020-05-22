
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>

#include "color.h"
#include "constants.h"
#include "geom.h"
#include "progress_bar.h"
#include "rand.h"
#include "ray.h"
#include "scene.h"

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

  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / ASPECT_RATIO);

  std::vector<std::array<int, 3>> img_data(image_width*image_width);

  // image generation
  auto scene = Scene::create("book_cover");
  ProgressBar pb{};

  #pragma omp parallel for shared(img_data, scene) schedule(dynamic)
  for (int j = image_height - 1; j >= 0; --j) {
    pb.set_value(100.0 * (image_height - 1 - j) / (double)image_height);
    for (int i = 0; i < image_width; ++i) {
      Color pixel_color{0.0, 0.0, 0.0};
      for (int s = 0; s < SAMPLES_PER_PIXEL; ++s) {
        double u = (i + nrand()) / (image_width - 1);
        double v = (j + nrand()) / (image_height - 1);
        Ray r = scene.camera().get_ray(u, v);
        pixel_color += ray_color(r, scene.objects());
      }
      img_data[(image_height - 1 - j) * image_width + i] = gen_color(pixel_color, SAMPLES_PER_PIXEL);
    }
  }

  // write image file
  std::ofstream outfile("image.ppm");

  // write ppm header
  outfile << "P3\n" << image_width << " " << image_height << "\n" << IRGB_MAX << "\n";

  for (int j = 0; j <= image_height - 1; ++j) {
    for (int i = 0; i < image_width; ++i) {
      write_color(outfile, img_data[j * image_width + i]);
    }
  }

  return 0;
}