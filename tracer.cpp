
#include <iostream>

#include "color.h"
#include "constants.h"
#include "geom.h"
#include "ray.h"

Color ray_color(const Ray& r) {
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
}

int main() {
    const int image_width = 256;
    const int image_height = 256;
    const int irgb_max = 255;
    std::cout << "P3\n" << image_width << " " << image_height << "\n" << irgb_max << "\n";

    // viewport parameters
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    // camera location
    Point3 origin{0, 0, 0};
    Vec3 horizontal{viewport_width, 0, 0};
    Vec3 vertical{0, viewport_height, 0};

    // lower left corner
    auto llc = origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length);

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << std::flush;
        for (int i = 0; i < image_width; ++i) {
            double u = double(i) / (image_width - 1);
            double v = double(j) / (image_height - 1);
            Ray r{origin, llc + u * horizontal + v * vertical - origin};
            Color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone\n";
    return 0;
}