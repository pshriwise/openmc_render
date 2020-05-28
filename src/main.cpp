
#include <fstream>
#include <memory>
#include <string>

#include "color.h"
#include "constants.h"
#include "geom.h"
#include "progress_bar.h"
#include "rand.h"
#include "ray.h"
#include "scene.h"
#include "teardown.h"
#include "trace.h"

int main(int argc, char** argv) {

  // options
  std::string scene_name = "three_spheres";

  if (argc > 1) {
    scene_name = argv[1];
  }

  int image_width = 200;


  if (argc > 2) {
    image_width = std::stoi(argv[2]);
  }

  int samples_per_pixel = 40;

  if (argc > 3) {
    samples_per_pixel = std::stoi(argv[3]);
  }

  const int image_height = static_cast<int>(image_width / ASPECT_RATIO);

  std::vector<std::array<uint8_t, 3>> img_data(image_width*image_width);

  // image generation
  Scene scene = create_scene(scene_name);
  ProgressBar pb{};

  for (int j = image_height - 1; j >= 0; --j) {
    pb.set_value(100.0 * (image_height - 1 - j) / (double)image_height);
    #pragma omp parallel for shared(img_data, scene) schedule(dynamic)
    for (int i = 0; i < image_width; ++i) {
      Color pixel_color{0.0, 0.0, 0.0};
      for (int s = 0; s < samples_per_pixel; ++s) {
        double u = (i + nrand()) / (image_width - 1);
        double v = (j + nrand()) / (image_height - 1);
        Ray r = scene.camera().get_ray(u, v);
        pixel_color += trace_color(r, scene.objects());
      }
      img_data[(image_height - 1 - j) * image_width + i] = gen_color(pixel_color, samples_per_pixel);
    }
  }

  // write image file
  std::ofstream outfile;
  outfile.open("image.ppm");

  // write ppm header
  outfile << "P6\n" << image_width << " " << image_height << "\n" << IRGB_MAX << "\n";
  outfile.close();

  outfile.open("image.ppm", std::ios::binary | std::ios::app);

  for (int j = 0; j <= image_height - 1; ++j) {
    for (int i = 0; i < image_width; ++i) {
      write_color(outfile, img_data[j * image_width + i]);
    }
  }
  outfile << '\n';
  outfile.close();


  // used to clean up openmc memory if needed
  teardown();

  return 0;
}