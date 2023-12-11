#pragma once

#include "interval.h"
#include "vec3.h"

#include <iostream>

using color = vec3;

inline void write_color(std::ostream &out, const color &pixel_color, const int samples_per_pixel)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    const auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(255.999 * intensity.clamp(r)) << ' '
        << static_cast<int>(255.999 * intensity.clamp(g)) << ' '
        << static_cast<int>(255.999 * intensity.clamp(b)) << '\n';
}
