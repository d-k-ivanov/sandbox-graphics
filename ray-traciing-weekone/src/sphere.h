#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : hittable
{
public:
    sphere(const point3 &center, const double radius)
        : m_center(center)
        , m_radius(radius)
    {
    }

    bool hit(const ray &r, const double ray_tmin, const double ray_tmax, hit_record &rec) override
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
        if(root <= ray_tmin || ray_tmax <= root)
        {
            root = (-half_b + sqrtd) / a;
            if(root <= ray_tmin || ray_tmax <= root)
            {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        const vec3 outward_normal = (rec.p - m_center) / m_radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

private:
    point3 m_center;
    double m_radius;
};
