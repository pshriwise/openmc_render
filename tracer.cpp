
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>

#include "camera.h"
#include "color.h"
#include "constants.h"
#include "geom.h"
#include "progress_bar.h"
#include "rand.h"
#include "ray.h"

Color ray_color(const Ray& r, const ObjectList& objects) {
  // copy of the input ray
  Ray ray = r;
  Color ray_color;

  std::vector<Color> stack;

  while(true) {

    if (stack.size() > MAX_BOUNCE) {
      stack.push_back(Color(0.0, 0.0, 0.0));
      break;
    }

    Hit hit;
    if (objects.hit(ray, 0.001, INFTY, hit)) {
      // scatter, add color contribution and continue to follow the ray
      if (hit.material_->scatter(ray, hit, ray_color, ray)) {
        stack.push_back(ray_color);
        continue;
      // absorption, add null color to the end of the stack
      } else {
        stack.push_back(Color(0.0, 0.0, 0.0));
        break;
      }
    }

    // ray reached the background, compute background value and add it to the stack
    Vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5*(unit_direction.y() + 1.0);
    stack.push_back((1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0));
    break;
  }

  if (stack.size() > 0) {
    auto color_out = std::accumulate(stack.begin(), stack.end(), Color(1.0, 1.0, 1.0), std::multiplies<Color>());
    return color_out;
  }

  return BLACK;
}

Color ray_color_inner(const Ray& r, const ObjectList& objects, int depth = 0) {
  Hit hit;

  if (depth > MAX_BOUNCE) {
    return Color(0,0,0);
  }

  if (objects.hit(r, 0.001, INFTY, hit)) {
    Ray scattered;
    Color attenuation;
    if (hit.material_->scatter(r, hit, attenuation, scattered)) {
      return attenuation * ray_color_inner(scattered, objects, ++depth);
    }
    return Color(0.0, 0.0, 0.0);
  }

  // Color using background
  Vec3 unit_direction = unit_vector(r.direction());
  double t = 0.5*(unit_direction.y() + 1.0);
  return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
}

struct Scene {
  ObjectList objects_;
  Camera camera_;

  inline Camera& camera() { return camera_; }
  inline ObjectList& objects() { return objects_; }
};

Scene three_spheres() {
  // Rendering objects
  ObjectList objects;
  auto material1 = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  auto material2 = std::make_shared<Lambertian>(Color(0.8 , 0.8, 0.0));

  auto material3 = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);
  //auto material4 = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 1.0);
  auto material4 = std::make_shared<Dielectric>(1.5);

  objects.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5, material1));
  objects.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100, material2));
  objects.add(std::make_shared<Sphere>(Point3(1.0, 0, -1), 0.5, material3));
  objects.add(std::make_shared<Sphere>(Point3(-1.0, 0, -1), 0.5, material4));

  // Setup camera
  Point3 camera_position{3, 3, 2};
  Point3 camera_target{0, 0, -1};
  double field_of_view = 20;
  double aperture = 2.0;

  Camera camera(camera_position,
                camera_target,
                Point3(0, 1, 0),
                field_of_view,
                ASPECT_RATIO,
                aperture,
                (camera_position - camera_target).length());

  return {objects, camera};
}

Scene red_blue() {
  ObjectList objects;

  double r = cos(PI/4);
  objects.add(std::make_shared<Sphere>(Point3(-r, 0, -1),
            r,
            std::make_shared<Lambertian>(Color(0,0,1))));
  objects.add(std::make_shared<Sphere>(Point3(r, 0, -1),
            r,
            std::make_shared<Lambertian>(Color(1, 0, 0))));

  // Setup camera
  Point3 camera_position{0, 0, 0};
  Point3 camera_target{0, 0, -1};
  double field_of_view = 90;
  double aperture = 0.0; // no defocus blur

  Camera camera(camera_position,
                camera_target,
                Point3(0, 1, 0),
                field_of_view,
                ASPECT_RATIO,
                aperture,
                (camera_position - camera_target).length());

  return {objects, camera};
}

Scene book_cover() {

  ObjectList objects;

  auto ground_mat = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  objects.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_mat));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = nrand();
      Point3 center{a + 0.9*nrand(), 0.2, b + 0.9*nrand()};

      if ((center - Vec3(4, 0.2, 0)).length() > 2.0) {
        if (choose_mat < 0.8) {
          Color albedo = Color::random(0, 1);
          auto sphere_material = std::make_shared<Lambertian>(albedo);
          objects.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          auto albedo = Color::random(0.5, 1);
          auto fuzz = nrand(0, 0.5);
          auto sphere_material = std::make_shared<Metal>(albedo, fuzz);
          objects.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          auto sphere_material = std::make_shared<Dielectric>(1.5);
          objects.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = std::make_shared<Dielectric>(1.5);
  objects.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  objects.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  objects.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  // Setup camera
  Point3 camera_position{13, 2, -3};
  Point3 camera_target{0, 0, 0};
  double field_of_view = 20;
  double aperture = 0.1;
  double focal_dist = 10.0;

  Camera camera(camera_position,
                camera_target,
                Point3(0, 1, 0),
                field_of_view,
                ASPECT_RATIO,
                aperture,
                10.0);
  return {objects, camera};
}

int main() {

  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / ASPECT_RATIO);

  std::vector<std::array<int, 3>> img_data(image_width*image_width);

  // image generation
  Scene scene = three_spheres();
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