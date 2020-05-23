
#ifndef COLOR_H
#define COLOR_H

#include <fstream>
#include <array>

#include "vec3.h"

// Some constant colors
const Color WHITE {1.0, 1.0, 1.0};
const Color BLACK {0.0, 0.0, 0.0};
const Color ABSORBED {0.0, 0.0, 0.0};

std::array<uint8_t, 3> gen_color( Color pixel_color, int n_samples);

void write_color(std::ofstream &out, std::array<uint8_t, 3> color);

#endif
