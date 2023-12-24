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

class dielectric : public material
{
public:
    dielectric(const double index_of_refraction)
        : m_ir(index_of_refraction)
    {
    }

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        const double refraction_ratio = rec.front_face ? (1.0 / m_ir) : m_ir;

        const vec3 unit_direction = unit_vector(r_in.direction());
        // const vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);
        // scattered = ray(rec.p, refracted);

        const double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        const bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        // if(cannot_refract)
        if(cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        {
            direction = reflect(unit_direction, rec.normal);
        }
        else
        {
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = ray(rec.p, direction);

        return true;
    }

private:
    double m_ir;    // Index of Refraction

    static double reflectance(const double cosine, const double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};
