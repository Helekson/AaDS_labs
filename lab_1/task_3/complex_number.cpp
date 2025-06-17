#include "complex_number.h"

const double complex_number::epsilon = 1e-10;

complex_number::complex_number(double r, double i) : real(r), img(i) {}

complex_number& complex_number::operator+=(const complex_number& rhs) {
    real += rhs.real;
    img += rhs.img;
    return *this;
}

complex_number& complex_number::operator+=(const complex_number& rhs) noexcept {
    real += rhs.real;
    img += rhs.img;
    return *this;
}

complex_number& complex_number::operator-=(const complex_number& rhs) noexcept {
    real -= rhs.real;
    img -= rhs.img;
    return *this;
}

complex_number& complex_number::operator*=(const complex_number& rhs) noexcept {
    const double new_real = real * rhs.real - img * rhs.img;
    const double new_img = real * rhs.img + img * rhs.real;
    real = new_real;
    img = new_img;
    return *this;
}

complex_number& complex_number::operator/=(const complex_number& rhs) {
    const double denominator = rhs.real * rhs.real + rhs.img * rhs.img;
    if (std::abs(denominator) < epsilon) {
        throw std::runtime_error("Деление на ноль");
    }
    
    const double new_real = (real * rhs.real + img * rhs.img) / denominator;
    const double new_img = (img * rhs.real - real * rhs.img) / denominator;
    real = new_real;
    img = new_img;
    return *this;
}

complex_number operator+(complex_number lhs, const complex_number& rhs) noexcept {
    return lhs += rhs;
}

complex_number operator-(complex_number lhs, const complex_number& rhs) noexcept {
    return lhs -= rhs;
}

complex_number operator*(complex_number lhs, const complex_number& rhs) noexcept {
    return lhs *= rhs;
}

complex_number operator/(complex_number lhs, const complex_number& rhs) {
    return lhs /= rhs;
}

std::ostream& operator<<(std::ostream& os, const complex_number& num) {
    os << '(' << num.real << (num.img >= 0 ? "+" : "") << num.img << "i)";
    return os;
}

std::istream& operator>>(std::istream& is, complex_number& num) {
    char ch;
    char sign;
    if (!(is >> ch) || ch != '(') is.setstate(std::ios::failbit);
    if (!(is >> num.real)) is.setstate(std::ios::failbit);
    if (!(is >> sign) || (sign != '+' && sign != '-')) 
    is.setstate(std::ios::failbit);
    if (!(is >> num.img)) is.setstate(std::ios::failbit);
    if (sign == '-') num.img *= -1;
    if (!(is >> ch) || ch != 'i') is.setstate(std::ios::failbit);
    if (!(is >> ch) || ch != ')') is.setstate(std::ios::failbit);
    return is;
}

double complex_number::abs() const {
    return std::sqrt(real * real + img * img);
}

double complex_number::arg() const {
    if (std::abs(real) < epsilon && std::abs(img) < epsilon) 
        throw std::domain_error("Аргумент нуля не определен");
    return std::atan2(img, real);
}
