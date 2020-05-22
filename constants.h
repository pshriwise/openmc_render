
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <limits>

using std::sqrt;

// Numerics
const double INFTY = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double deg_to_rad(double degrees) { return degrees * PI / 180.0; }
inline double rad_to_deg(double radians) { return radians * 180.0 / PI; }

// Image
const double ASPECT_RATIO = 16.0 / 9.0;
const int SAMPLES_PER_PIXEL = 30;

// Colors
const double RGB_MAX = 256;
const int IRGB_MAX = 255;

// Scene
const int MAX_BOUNCE = 50;

// Physics
const double GAMMA = 2.2;
const double RCP_GAMMA = 1.0 / GAMMA;

#endif