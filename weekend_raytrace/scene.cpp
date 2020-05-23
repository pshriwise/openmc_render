
#include "scene.h"

#ifdef OPENMC
#include "openmc/capi.h"
#endif

Scene create_scene(std::string scene_name) {
  if (scene_name == "red_blue") {
    return red_blue();
  } else if (scene_name == "three_spheres") {
    return three_spheres();
  } else if (scene_name == "book_cover") {
    return book_cover();
  } else if (scene_name == "openmc") {
    #ifdef OPENMC
    return openmc_setup();
    #else
    throw std::invalid_argument("OpenMC rendering is not enabled.");
    #endif
  } else {
    throw std::invalid_argument("Invalid scene name \"" + scene_name + "\" specified.");
  }
}

Scene three_spheres() {
  // Rendering objects
  ObjectList objects;
  auto material1 = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  auto material2 = std::make_shared<Lambertian>(Color(0.8 , 0.8, 0.0));

  auto material3 = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 1.0);
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
  Point3 camera_position{10, 2, -5};
  Point3 camera_target{0, 0, 0};
  double field_of_view = 30;
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

#ifdef OPENMC
Scene openmc_spheres() {

  ObjectList objects;

  auto material1 = std::make_shared<Lambertian>(Color(0.0, 0.0, 0.5));
  auto material2 = std::make_shared<Lambertian>(Color(0.5 , 0.5, 0.5));

  objects.add(std::make_shared<OpenMCSphere>(Point3(0, 0, -1), 0.5, material1));
  objects.add(std::make_shared<OpenMCSphere>(Point3(0, -100.5, -1), 100, material2));

  // Setup camera
  Point3 camera_position{0, 5, 0};
  Point3 camera_target{0, 0, -1};
  double field_of_view = 90;
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

Scene openmc_setup() {
  openmc_init(0, nullptr, nullptr);

  ObjectList objects;

  std::vector<std::shared_ptr<Material>> materials;

  // create a couple of materials
  materials.push_back(std::make_shared<Metal>(Color(0.5, 0.5, 0.5), 0.0));
  materials.push_back(std::make_shared<Lambertian>(Color(0.0, 0.2, 0.8)));

  // add more materials as needed
  while(materials.size() < openmc::model::cells.size()) {
    materials.push_back(std::make_shared<Lambertian>(Color::random()));
  }

  // create an object for each cell in the OpenMC problem

  for (int i = 0; i < openmc::model::cells.size(); ++i) {
    // randomize material
    const auto& cell = openmc::model::cells[i];

    auto object = std::make_shared<OpenMCCell>(cell.get(), materials[i]);

    objects.add(object);
  }

  // Setup camera
  Point3 camera_position{0, 0.25, 1};
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
#endif //OPENMC