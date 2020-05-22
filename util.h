
#ifndef UTIL_H
#define UTIL_H

#include <algorithm>

inline double clamp(double x, double min, double max) {
    return std::min(std::max(x, min), max);
}

#endif