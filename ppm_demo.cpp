
#include <iostream>

int main() {
    const int image_width = 256;
    const int image_height = 256;
    const int irgb_max = 255;
    const double rgb_max = 255.999;
    std::cout << "P3\n" << image_width << " " << image_height << "\n" << irgb_max << "\n";

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            double r = double(i) / (image_width - 1);
            double g = double(j) / (image_height - 1);
            double b = 0.25;

            int ir = static_cast<int>(rgb_max * r);
            int ig = static_cast<int>(rgb_max * g);
            int ib = static_cast<int>(rgb_max * b);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    return 0;
}