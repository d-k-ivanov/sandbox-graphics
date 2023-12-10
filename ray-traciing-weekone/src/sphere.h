#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
    sphere(const point3 &center, const double radius)
        : m_center(center)
        , m_radius(radius)
    {
    }

    // bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &rec) const override
    bool hit(const ray &r, const interval ray_t, hit_record &rec) const override
    {
        const vec3 oc = r.origin() - m_center;
        const auto a = r.direction().length_squared();
        const auto half_b = dot(oc, r.direction());
        const auto c = oc.length_squared() - m_radius * m_radius;

        const auto discriminant = half_b * half_b - a * c;
        if(discriminant < 0)
        {
            return false;
        }
        const auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        // if(root <= ray_tmin || ray_tmax <= root)
        if(!ray_t.surrounds(root))
        {
            root = (-half_b + sqrtd) / a;
            // if(root <= ray_tmin || ray_tmax <= root)
            if(!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - m_center) / m_radius;

        return true;
    }

private:
    point3 m_center;
    double m_radius;
};
