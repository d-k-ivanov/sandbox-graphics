#include "color.h"
#include "vec3.h"

#include <iostream>

int main()
{
    // Image
    constexpr int image_width  = 256;
    constexpr int image_height = 256;

    // Render
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    for(int j = 0; j < image_height; ++j)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for(int i = 0; i < image_width; ++i)
        {
            // Generate colours:
            const auto r = static_cast<double>(i) / (image_width - 1);
            const auto g = static_cast<double>(j) / (image_height - 1);
            const auto b = 0;

            const auto pixel_color = color(r, g, b);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}
