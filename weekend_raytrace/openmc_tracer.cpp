
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "color.h"
#include "constants.h"
#include "geom.h"
#include "progress_bar.h"
#include "rand.h"
#include "ray.h"
#include "scene.h"

#include "openmc/surface.h"

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

class OpenMCSphere : public Object {
public:
  OpenMCSphere(Point3 center, double radius, std::shared_ptr<Material> material)
    : material_(material) {

      // create an OpenMC object for the sphere

      openmc_sphere_ = std::make_shared<openmc::SurfaceSphere>();
      openmc_sphere_->x0_ = center.x();
      openmc_sphere_->y0_ = center.y();
      openmc_sphere_->z0_ = center.z();
      openmc_sphere_->radius_ = radius;

  };

  // Methods
  virtual bool hit(const Ray& r, double t_min, double t_max, Hit& rec) const;

  std::shared_ptr<Material> material_;
  std::shared_ptr<openmc::SurfaceSphere> openmc_sphere_;
};

bool OpenMCSphere::hit (const Ray& r, double t_min, double t_max, Hit& rec) const {

  // check for a hit
  openmc::Position p = r.orig.e;
  openmc::Direction u = r.dir.e;
  u /= u.norm();
  double dist = openmc_sphere_->distance(p, u, false);

  if (dist < openmc::INFTY) {
    // compute t-value
    rec.t_ = dist;
    rec.p_ = r.at(dist);
    auto openmc_norm = -openmc_sphere_->normal(rec.p_.e);
    Vec3 outward_normal{openmc_norm.x, openmc_norm.y, openmc_norm.z};
    rec.set_face_normal(r, unit_vector(outward_normal));
    rec.material_ = material_;
    return true;
  }

  return false;
}

Scene openmc_spheres() {

  ObjectList objects;

  auto material1 = std::make_shared<Lambertian>(Color(0.0, 0.0, 0.5));
  auto material2 = std::make_shared<Lambertian>(Color(0.5 , 0.5, 0.5));

  objects.add(std::make_shared<OpenMCSphere>(Point3(0, 0, -1), 0.5, material1));
  objects.add(std::make_shared<OpenMCSphere>(Point3(0, -100.5, -1), 100, material2));

  // Setup camera
  Point3 camera_position{0, 0, 0};
  Point3 camera_target{0, 0, -1};
  double field_of_view = 90;
  double aperture = 0.0;

  Camera camera(camera_position,
                camera_target,
                Point3(0, 1, 0),
                field_of_view,
                ASPECT_RATIO,
                aperture,
                (camera_position - camera_target).length());

  return {objects, camera};
}

int main(int argc, char** argv) {

  std::string scene_name = "three_spheres";

  if (argc > 1) {
    scene_name = argv[1];
  }

  int image_width = 200;

  if (argc > 2) {
    image_width = std::stoi(argv[2]);
  }

  const int image_height = static_cast<int>(image_width / ASPECT_RATIO);

  std::vector<std::array<uint8_t, 3>> img_data(image_width*image_width);

  // image generation
  //auto scene = Scene::create(scene_name);
  auto scene = openmc_spheres();
  ProgressBar pb{};

  for (int j = image_height - 1; j >= 0; --j) {
    pb.set_value(100.0 * (image_height - 1 - j) / (double)image_height);
    #pragma omp parallel for shared(img_data, scene) schedule(dynamic)
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

  return 0;
}