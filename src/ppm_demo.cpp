
#include <iostream>

#include "color.h"
#include "constants.h"

int main() {
    const int image_width = 256;
    const int image_height = 256;
    const int irgb_max = 255;
    std::cout << "P3\n" << image_width << " " << image_height << "\n" << irgb_max << "\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << std::flush;
        for (int i = 0; i < image_width; ++i) {
            double r = double(i) / (image_width - 1);
            double g = double(j) / (image_height - 1);
            double b = 0.25;

            Color pixel_color = {r, g, b};
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone\n";
    return 0;
}