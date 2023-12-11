// ReSharper disable CppClangTidyModernizeUseNodiscard
#pragma once

#include "utils.h"

class interval
{
public:
    double min, max;

    interval()
        : min(+infinity)
        , max(-infinity)
    {
    }    // Default interval is empty

    interval(const double from, const double to)
        : min(from)
        , max(to)
    {
    }

    bool contains(const double x) const
    {
        return min <= x && x <= max;
    }

    bool surrounds(const double x) const
    {
        return min < x && x < max;
    }

    double clamp(const double x) const
    {
        if(x < min)
        {
            return min;
        }
        if(x > max)
        {
            return max;
        }
        return x;
    }

    static const interval empty, universe;
};

const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);
