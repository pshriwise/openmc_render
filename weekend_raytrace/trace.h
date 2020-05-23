
#ifndef TRACE_H
#define TRACE_H

#include "color.h"
#include "ray.h"
#include "geom.h"

Color trace_color(const Ray& r, const ObjectList& objects);

#endif