#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hit_sphere(const point3 &center, double radius, const ray &r)
{
    const vec3 oc = r.origin() - center;
    const auto a = dot(r.direction(), r.direction());
    const auto b = 2.0 * dot(oc, r.direction());
    const auto c = dot(oc, oc) - radius * radius;
    const auto discriminant = b * b - 4 * a * c;
    if(discriminant < 0)
    {
        return -1.0;
    }
    return (-b - sqrt(discriminant)) / (2.0 * a);
}

color ray_color(const ray &r, int pixel_w)
{
    const auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if(t > 0.0)
    {
        const vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }
    // blendedValue = (1 − a) * startValue + a * endValue
    const vec3 unit_direction = unit_vector(r.direction());
    const auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + (pixel_w % 2 == 0 ? a * color(0.5, 0.7, 1.0) : a * color(0.5, 1.0, 0.7));
}

int main()
{
    // Aspect ratios:
    // width / height =  2 / 1 = 2
    // width / height = 16 / 9 = 1.7778
    // Image
    // constexpr int image_width = 256;
    // constexpr int image_height = 256;

    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    constexpr auto focal_length = 1.0;
    constexpr auto viewport_height = 2.0;
    const auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    const auto camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const auto viewport_u = vec3(viewport_width, 0, 0);
    const auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    const auto pixel_delta_u = viewport_u / image_width;
    const auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    const auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    const auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n";
    std::cout << image_width << ' ' << image_height << "\n";
    std::cout << "255\n";

    for(int j = 0; j < image_height; ++j)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for(int i = 0; i < image_width; ++i)
        {
            // Generate colours:
            // const auto r = static_cast<double>(i) / (image_width - 1);
            // const auto g = static_cast<double>(j) / (image_height - 1);
            // const auto b = 0;
            // const auto pixel_color = color(r, g, b);
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            const color pixel_color = ray_color(r, i);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}
