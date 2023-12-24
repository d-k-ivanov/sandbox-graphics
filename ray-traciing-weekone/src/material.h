#pragma once

#include "color.h"
#include "hittable.h"
#include "ray.h"

class hit_record;

class material
{
public:
    virtual ~material() = default;

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const color &color)
        : m_albedo(color)
    {
    }

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if(scatter_direction.near_zero())
        {
            scatter_direction = rec.normal;
        }
        scattered = ray(rec.p, scatter_direction);
        attenuation = m_albedo;
        return true;
    }

private:
    color m_albedo;
};

class metal : public material
{
public:
    metal(const color &color, const double fuzz)
        : m_albedo(color)
        , m_fuzz(fuzz < 1 ? fuzz : 1)
    {
    }

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        const vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + m_fuzz * random_unit_vector());
        attenuation = m_albedo;
        // return true;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color m_albedo;
    double m_fuzz;
};
