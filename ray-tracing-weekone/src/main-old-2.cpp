#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include <memory>

int main()
{
    hittable_list world;

    // Metals
    // auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = std::make_shared<lambertian>(color(0.7, 0.3, 0.3));
    // auto material_left   = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    // auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // Dielectric 1:
    // auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = std::make_shared<dielectric>(1.5);
    // auto material_left   = std::make_shared<dielectric>(1.5);
    // auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // Dielectric 2:
    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<dielectric>(1.5);
    auto material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(std::make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
    world.add(std::make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera Test
    // auto r = cos(pi / 4);
    // auto material_left = std::make_shared<lambertian>(color(0, 0, 1));
    // auto material_right = std::make_shared<lambertian>(color(1, 0, 0));
    //
    // world.add(std::make_shared<sphere>(point3(-r, 0, -1), r, material_left));
    // world.add(std::make_shared<sphere>(point3(r, 0, -1), r, material_right));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    // cam.vfov     = 90;
    cam.vfov = 20;
    cam.lookfrom = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    // cam.defocus_angle = 1.0;
    // cam.focus_dist = 1.0;
    cam.defocus_angle = 10.0;
    cam.focus_dist = 3.4;

    cam.render(world);
}
