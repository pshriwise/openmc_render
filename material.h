
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
  Lambertian(const Color &a ) : albedo_(a) {};

  virtual bool scatter(const Ray& r, const Hit& hit, Color& attenuation, Ray& scattered) const {
    Vec3 scatter_dir = hit.n_ + Vec3::random();
    scattered = Ray(hit.p_, scatter_dir);
    attenuation = albedo_;
    return true;
  }

  // Data members
  Color albedo_;

};
#endif