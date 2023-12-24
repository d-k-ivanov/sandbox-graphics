// ReSharper disable CppClangTidyModernizeUseNodiscard
// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable CppTooWideScopeInitStatement
#pragma once

#include "color.h"
#include "hittable.h"
#include "material.h"

class camera
{
public:
    double aspect_ratio = 1.0;     // Ratio of image width over height
    int image_width = 100;         // Rendered image width in pixel count
    int samples_per_pixel = 10;    // Count of random samples for each pixel
    int max_depth = 10;            // Maximum number of ray bounces into scene
    double vfov = 90;              // Vertical view angle (field of view)

    void render(const hittable &world)
    {
        initialize();

        std::cout << "P3\n";
        std::cout << image_width << ' ' << m_image_height << "\n";
        std::cout << "255\n";

        for(int j = 0; j < m_image_height; ++j)
        {
            std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' ' << std::flush;
            for(int i = 0; i < image_width; ++i)
            {
                // auto pixel_center = m_pixel00_loc + (i * m_pixel_delta_u) + (j * m_pixel_delta_v);
                // auto ray_direction = pixel_center - m_center;
                // ray r(m_center, ray_direction);
                // const color pixel_color = ray_color(r, world);
                // write_color(std::cout, pixel_color);
                color pixel_color(0, 0, 0);
                for(int sample = 0; sample < samples_per_pixel; ++sample)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }
        std::clog << "\rDone.                 \n";
    }

private:
    int m_image_height = 0;    // Rendered image height
    point3 m_center;           // Camera m_center
    point3 m_pixel00_loc;      // Location of pixel 0, 0
    vec3 m_pixel_delta_u;      // Offset to pixel to the right
    vec3 m_pixel_delta_v;      // Offset to pixel below

    void initialize()
    {
        m_image_height = static_cast<int>(image_width / aspect_ratio);
        m_image_height = (m_image_height < 1) ? 1 : m_image_height;

        m_center = point3(0, 0, 0);

        // Determine viewport dimensions.
        constexpr auto focal_length = 1.0;
        const auto theta = degrees_to_radians(vfov);    // 90 deg == 0.5pi == 1.5707963268 rad
        const auto h = tan(theta / 2);                  // tan(1.5707963268/2) == 1
        // constexpr auto viewport_height = 2.0;
        const auto viewport_height = 2 * h * focal_length;    // 2 * h * focal_length == 2.0
        const auto viewport_width = viewport_height * (static_cast<double>(image_width) / m_image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        const auto viewport_u = vec3(viewport_width, 0, 0);
        const auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        m_pixel_delta_u = viewport_u / image_width;
        m_pixel_delta_v = viewport_v / m_image_height;

        // Calculate the location of the upper left pixel.
        const auto viewport_upper_left = m_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        m_pixel00_loc = viewport_upper_left + 0.5 * (m_pixel_delta_u + m_pixel_delta_v);
    }

    // Get a randomly sampled camera ray for the pixel at location i,j.
    ray get_ray(const int i, const int j) const
    {
        const auto pixel_center = m_pixel00_loc + (i * m_pixel_delta_u) + (j * m_pixel_delta_v);
        const auto pixel_sample = pixel_center + pixel_sample_square();
        // const auto pixel_sample = pixel_center + random_in_unit_disk();

        const auto ray_origin = m_center;
        const auto ray_direction = pixel_sample - ray_origin;

        return {ray_origin, ray_direction};
    }

    // Returns a random point in the square surrounding a pixel at the origin.
    vec3 pixel_sample_square() const
    {
        const auto px = -0.5 + random_double();
        const auto py = -0.5 + random_double();
        return (px * m_pixel_delta_u) + (py * m_pixel_delta_v);
    }

    // Generate a sample from the disk of given radius around a pixel at the origin.
    // TBA
    // vec3 pixel_sample_disk(double radius) const
    // {
    //     auto p = radius * random_in_unit_disk();
    //     return (p[0] * m_pixel_delta_u) + (p[1] * m_pixel_delta_v);
    // }

    color ray_color(const ray &r, const int depth, const hittable &world) const
    {
        hit_record rec;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if(depth <= 0)
        {
            return {0, 0, 0};
        }

        // if(world.hit(r, interval(0, 0.55), rec))
        if(world.hit(r, interval(0.001, infinity), rec))
        {
            // return 0.5 * (rec.normal + color(1, 1, 1));
            // const vec3 direction = random_on_hemisphere(rec.normal);
            // const vec3 direction = rec.normal + random_unit_vector();
            // return 0.5 * ray_color(ray(rec.p, direction), depth - 1, world);

            ray scattered;
            color attenuation;
            if(rec.material->scatter(r, rec, attenuation, scattered))
            {
                return attenuation * ray_color(scattered, depth - 1, world);
            }
            return {0, 0, 0};
        }

        const vec3 unit_direction = unit_vector(r.direction());
        const auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        // return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(1.0, 0.0, 0.0);
    }
};
