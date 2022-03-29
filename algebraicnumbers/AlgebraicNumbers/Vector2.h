#pragma once

#include <iostream>

class Vector2
{
public:
    float x;
    float y;
    Vector2();
    Vector2(const float& a, const float& b);
    Vector2 operator+(const Vector2& b) const; //vector addition
    Vector2& operator+=(const Vector2& b);
    Vector2 operator-(const Vector2& b) const; //vector subtraction
    Vector2& operator-=(const Vector2& b);
    Vector2 operator*(const float& b) const; //vector scalar multiplication
    Vector2& operator*=(const float& b);
    Vector2 operator/(const float& b) const; //vector scalar division
    Vector2& operator/=(const float& b);
    float operator*(const Vector2& b) const; //dot product
    void normalize();
    void normalize(const float& dist);
    [[nodiscard]] Vector2 normalized() const;
    [[nodiscard]] Vector2 normalized(const float& dist) const;
    [[nodiscard]] float length() const;
};

Vector2 operator*(const float& b, const Vector2& c);
std::ostream& operator<<(std::ostream& out, const Vector2& c);

Vector2 lerp(double t, const Vector2& vec1, const Vector2& vec2);
Vector2 qerp(double t, const Vector2& vec0, const Vector2& vec1, const Vector2& vec2, const Vector2& vec3);
