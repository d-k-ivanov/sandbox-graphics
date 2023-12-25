#pragma once

#include <limits>
#include <random>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(const double degrees)
{
    return degrees * pi / 180.0;
}

// Returns a random real in [0,1).
inline double random_double()
{
    std::random_device rand_device;
    std::mt19937 generator(rand_device());
    std::uniform_real_distribution<> distribution(0.0, 1.0);
    return distribution(generator);
    // return rand() / (RAND_MAX + 1.0);
}

// Returns a random real in [min,max)
inline double random_double(const double min, const double max)
{
    return min + (max - min) * random_double();
}
