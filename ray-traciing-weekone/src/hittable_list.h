#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list : public hittable
{
public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() = default;
    hittable_list(const std::shared_ptr<hittable> &object)
    {
        add(object);
    }

    void clear()
    {
        objects.clear();
    }

    void add(const std::shared_ptr<hittable> &object)
    {
        objects.push_back(object);
    }

    bool hit(const ray &r, const double ray_tmin, const double ray_tmax, hit_record &rec) override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;

        for(const auto &object : objects)
        {
            if(object->hit(r, ray_tmin, closest_so_far, temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};
