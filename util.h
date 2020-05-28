
#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <cmath>

inline double clamp(double x, double min, double max) {
    return std::min(std::max(x, min), max);
}

inline double schlick(double cosine, double ref_idx) {
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0)*std::pow(1 - cosine, 5);
}

#endif