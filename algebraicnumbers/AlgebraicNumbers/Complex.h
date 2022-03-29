#pragma once

#include <iostream>

class Complex
{
public:
    double x;
    double y;
    Complex();
    Complex(const double& a, const double& b);

    //Complex <-> Complex operators
    Complex operator+(const Complex& b) const;
    Complex& operator+=(const Complex& b);
    Complex operator-(const Complex& b) const;
    Complex& operator-=(const Complex& b);
    Complex operator*(const Complex& b) const;
    Complex& operator*=(const Complex& b);
    Complex operator/(const Complex& b) const;
    Complex& operator/=(const Complex& b);

    //Complex <-> Real operators
    Complex operator+(const double& b) const;
    Complex& operator+=(const double& b);
    Complex operator-(const double& b) const;
    Complex& operator-=(const double& b);
    Complex operator*(const double& b) const;
    Complex& operator*=(const double& b);
    Complex operator/(const double& b) const;
    Complex& operator/=(const double& b);
    [[nodiscard]] double length() const;
    [[nodiscard]] double length2() const; //returns the squared magnitude of the complex number
    [[nodiscard]] double arg() const; //returns the complex argument of the vector (direct call to atan2)
};

Complex operator*(const double& b, const Complex& c);
Complex operator/(const double& b, const Complex& c);
Complex operator+(const double& b, const Complex& c);
Complex operator-(const double& b, const Complex& c);
std::ostream& operator<<(std::ostream& out, const Complex& c);
const Complex I = Complex(0, 1);
