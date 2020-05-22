
#ifndef COLOR_H
#define COLOR_H

#include "constants.h"
#include "util.h"
#include "vec3.h"

#include <iostream>

using Color = Vec3;

void write_color(std::ostream &out, Color pixel_color, int n_samples) {
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // Divide by the total number of samples per pixel
    auto rcp_samples = 1.0 / n_samples;
    r *= rcp_samples;
    g *= rcp_samples;
    b *= rcp_samples;
    // Gamma correction: adjusting for perceived brightness vs. linear brightness
    r = std::pow(r, RCP_GAMMA);
    g = std::pow(g, RCP_GAMMA);
    b = std::pow(b, RCP_GAMMA);

    const double color_min = 0.0;
    const double color_max = 0.999;
    out << static_cast<int>(RGB_MAX * clamp(r, color_min, color_max)) << ' '
        << static_cast<int>(RGB_MAX * clamp(g, color_min, color_max)) << ' '
        << static_cast<int>(RGB_MAX * clamp(b, color_min, color_max)) << '\n';
}

#endif
