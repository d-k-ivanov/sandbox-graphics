#include "Vector2.h"

#include <iostream>

Vector2::Vector2()
{
    x = y = 0.0f;
}
Vector2::Vector2(const float& a,const float& b)
{
    x=a;
    y=b;
}
Vector2 Vector2::operator+ (const Vector2& b) const
{
    Vector2 temp;
    temp.x=x+b.x;
    temp.y=y+b.y;
    return temp;
}
Vector2& Vector2::operator+= (const Vector2& b)
{
    if(this!=&b)
    {
        x=x+b.x;
        y=y+b.y;
    }
    return *this;
}
Vector2 Vector2::operator-(const Vector2& b) const
{
    Vector2 temp;
    temp.x=x-b.x;
    temp.y=y-b.y;
    return temp;
}
Vector2& Vector2::operator-=(const Vector2& b)
{
    if(this!=&b)
    {
        x=x-b.x;
        y=y-b.y;
    }
    return *this;
}
float Vector2::operator* (const Vector2& b) const
{
    return (x*b.x+y*b.y);
}
Vector2 Vector2::operator* (const float& b) const
{
    Vector2 temp;
    temp.x=x*b;
    temp.y=y*b;
    return temp;
}
Vector2& Vector2::operator*= (const float& b)
{
    x=x*b;
    y=y*b;
    return *this;
}
Vector2 Vector2::operator/ (const float& b) const
{
    Vector2 temp=Vector2(0.0f,0.0f);
    if(b>0)
    {
        temp.x=x/b;
        temp.y=y/b;
    }
    return temp;
}
Vector2& Vector2::operator/= (const float& b)
{
    if(b>0)
    {
        x=x/b;
        y=y/b;
    }
    return *this;
}
void Vector2::normalize()
{
    float dist = sqrt(x*x+y*y);
    x/=dist;
    y/=dist;
}
void Vector2::normalize(const float& dist)
{
    x/=dist;
    y/=dist;
}
Vector2 Vector2::normalized() const
{
    float dist = sqrt(x*x+y*y);
    return Vector2(x/dist,y/dist);
}
Vector2 Vector2::normalized(const float& dist) const
{
    return Vector2(x/dist,y/dist);
}
float Vector2::length() const
{
    return sqrt(x*x+y*y);
}

std::ostream& operator<<(std::ostream& out, const Vector2& c)
{
    out << '(' << c.x << ',' << c.y << ')' ;
    return out;
}
Vector2 operator* (const float& b, const Vector2& c)
{
    Vector2 temp;
    temp.x=c.x*b;
    temp.y=c.y*b;
    return temp;
}
Vector2 lerp(double t,const Vector2& vec1,const Vector2& vec2)
{
    return (vec2-vec1)*t+vec1;
}
Vector2 qerp(double t,const Vector2& vec0,const Vector2& vec1,const Vector2& vec2,const Vector2& vec3)
{
    double t2=t*t;
    Vector2 a0=vec3-vec2-vec0+vec1;
    Vector2 a1=vec0-vec1-a0;
    Vector2 a2=vec2-vec0;
    return a0*t*t2+a1*t2+a2*t+vec1;
}
