#ifndef COMPLEX_NUMBER_H
#define COMPLEX_NUMBER_H

#include <iostream>
#include <cmath>
#include <stdexcept>

class complex_number {
private:
    double real;
    double img;
    static const double epsilon; 

public:
    complex_number(double r = 0.0, double i = 0.0);
    complex_number(const complex_number& other) = default;
    
    double get_real() const noexcept { return real; }
    double get_img() const noexcept { return img; }
    
    complex_number& operator=(const complex_number&) = default;
    
    complex_number& operator+=(const complex_number& rhs);
    complex_number& operator-=(const complex_number& rhs);
    complex_number& operator*=(const complex_number& rhs);
    complex_number& operator/=(const complex_number& rhs);
    
    complex_number operator+() const { return *this; }
    complex_number operator-() const { return {-real, -img}; }
    
    double abs() const; 
    double arg() const; 
    
    friend std::ostream& operator<<(std::ostream& os, const complex_number& num);
    friend std::istream& operator>>(std::istream& is, complex_number& num);
};

complex_number operator+(complex_number lhs, const complex_number& rhs);
complex_number operator-(complex_number lhs, const complex_number& rhs);
complex_number operator*(complex_number lhs, const complex_number& rhs);
complex_number operator/(complex_number lhs, const complex_number& rhs);

#endif
