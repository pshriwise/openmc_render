
#ifndef MATERIAL_H
#define MATERIAL

#include "vec3.h"

class Material {
public:
  virtual bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
  // Constructors
  Lambertian(const Color &a) : albedo_(a) {};

  virtual bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const {
    Vec3 scattered_dir = hit.n_ + Vec3::random();
    scattered = Ray(hit.p_, scattered_dir);
    attenuation = albedo_;
    return true;
  }

  // Data members
  Color albedo_;
};

class Metal : public Material {
public:
  // Constructors
  Metal(const Color &a, double fuzz = 0) : albedo_(a), fuzz_(fuzz) {};

  virtual bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const {
    Vec3 reflected_dir = reflect(r.direction(), hit.n_);
    scattered = Ray(hit.p_, reflected_dir + fuzz_ * Vec3::random_in_unit_sphere());
    attenuation = albedo_;
    return (dot(scattered.direction(), hit.n_) > 0.0);
  }

  // Data members
  Color albedo_;
  double fuzz_;
};

class Dielectric : public Material {
public:
  // Constructors
  Dielectric(double refraction_index) : ref_idx_(refraction_index) {};

  // Methods
  virtual bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const {
    attenuation = Color(1.0, 1.0, 1.0);
    double eta_ratio;
    if (hit.front_face_) {
      eta_ratio = 1.0 / ref_idx_;
    } else {
      eta_ratio = ref_idx_;
    }

    Vec3 unit_dir = unit_vector(r.direction());
    Vec3 refracted = refract(unit_dir, hit.n_, eta_ratio);
    scattered = Ray(hit.p_, refracted);
    return true;
  }

  // Data members
  double ref_idx_;
};

#endif