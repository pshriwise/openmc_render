


#ifndef GEOM_H
#define GEOM_H

#include "ray.h"

bool hit_sphere(const Point3& center, double radius, const Ray& r, double& t);

#endif