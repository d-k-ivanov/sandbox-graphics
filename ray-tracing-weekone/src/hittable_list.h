#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list : public hittable
{
public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() = default;

    explicit hittable_list(const std::shared_ptr<hittable> &object)
    {
        add(object);
    }

    void clear()
    {
        objects.clear();
    }

    void add(const std::shared_ptr<hittable> &object)
    {
        objects.emplace_back(object);
    }

    // bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &rec) const override
    bool hit(const ray &r, const interval ray_t, hit_record &rec) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for(const auto &object : objects)
        {
            if(object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};
