// ReSharper disable CppClangTidyModernizeUseNodiscard
#pragma once

#include <cmath>
#include <iostream>

class vec3
{
public:
    double e[3];

    vec3()
        : e{0, 0, 0}
    {
    }
    vec3(const double e0, const double e1, const double e2)
        : e{e0, e1, e2}
    {
    }

    double x() const
    {
        return e[0];
    }

    double y() const
    {
        return e[1];
    }

    double z() const
    {
        return e[2];
    }

    vec3 operator-(const vec3 &other) const
    {
        return {e[0] - other.e[0],
                e[1] - other.e[1],
                e[2] - other.e[2]};
    }

    void operator-=(const vec3 &other)
    {
        e[0] -= other.e[0];
        e[1] -= other.e[1];
        e[2] -= other.e[2];
    }

    vec3 operator-() const
    {
        return {-e[0], -e[1], -e[2]};
    }

    double operator[](const int i) const
    {
        return e[i];
    }

    double &operator[](const int i)
    {
        return e[i];
    }

    vec3 &operator+=(const vec3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3 &operator*=(const double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3 &operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    double length() const
    {
        return sqrt(length_squared());
    }

    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    // Return true if the vector is close to zero in all dimensions.
    bool near_zero() const
    {
        constexpr auto tolerance = 1e-8;
        return (fabs(e[0]) < tolerance) && (fabs(e[1]) < tolerance) && (fabs(e[2]) < tolerance);
    }

    static vec3 random()
    {
        return {random_double(), random_double(), random_double()};
    }

    static vec3 random(const double min, const double max)
    {
        return {random_double(min, max), random_double(min, max), random_double(min, max)};
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;

// Vector Utility Functions

inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return {u.e[0] + v.e[0],
            u.e[1] + v.e[1],
            u.e[2] + v.e[2]};
}

// inline vec3 operator-(const vec3 &u, const vec3 &v)
// {
//     return {u.e[0] - v.e[0],
//             u.e[1] - v.e[1],
//             u.e[2] - v.e[2]};
// }

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return {u.e[0] * v.e[0],
            u.e[1] * v.e[1],
            u.e[2] * v.e[2]};
}

inline vec3 operator*(const double t, const vec3 &v)
{
    return {t * v.e[0],
            t * v.e[1],
            t * v.e[2]};
}

inline vec3 operator*(const vec3 &v, double t)
{
    return t * v;
}

inline vec3 operator/(const vec3 &v, const double t)
{
    return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1],
            u.e[2] * v.e[0] - u.e[0] * v.e[2],
            u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline vec3 unit_vector(const vec3 &v)
{
    return v / v.length();
}

inline vec3 random_in_unit_sphere()
{
    while(true)
    {
        auto p = vec3::random(-1, 1);
        if(p.length_squared() < 1)
        {
            return p;
        }
    }
}

inline vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3 &normal)
{
    const vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere, normal) > 0.0)    // In the same hemisphere as the normal
    {
        return on_unit_sphere;
    }
    else
    {
        return -on_unit_sphere;
    }
}

vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}
