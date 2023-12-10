#pragma once

#include <cmath>
#include <limits>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(const double degrees)
{
    return degrees * pi / 180.0;
}
