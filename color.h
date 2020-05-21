
#ifndef COLOR_H
#define COLOR_H

#include "constants.h"
#include "vec3.h"

#include <iostream>

using Color = vec3;

void write_color(std::ostream &out, Color pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(rgb_max * pixel_color.x()) << ' '
        << static_cast<int>(rgb_max * pixel_color.y()) << ' '
        << static_cast<int>(rgb_max * pixel_color.z()) << '\n';
}

#endif
