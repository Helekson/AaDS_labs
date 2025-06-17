#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>
#include <stdexcept>
#include "LongInt.h"

class Fraction {
private:
    LongInt numerator;
    LongInt denominator;

    static LongInt absLI(const LongInt& x);
    static LongInt gcdLI(LongInt a, LongInt b);
    void reduce();
    static LongInt makeLongFromULL(unsigned long long v);

public:
    Fraction();
    Fraction(const LongInt& num, const LongInt& den);
    Fraction(int v);
    Fraction(const Fraction& other);
    Fraction& operator=(const Fraction& other);

    Fraction& operator+=(const Fraction& rhs);
    Fraction operator+(const Fraction& rhs) const;
    Fraction& operator-=(const Fraction& rhs);
    Fraction operator-(const Fraction& rhs) const;
    Fraction& operator*=(const Fraction& rhs);
    Fraction operator*(const Fraction& rhs) const;
    Fraction& operator/=(const Fraction& rhs);
    Fraction operator/(const Fraction& rhs) const;

    Fraction& operator++();
    Fraction operator++(int);
    Fraction& operator--();
    Fraction operator--(int);

    bool operator==(const Fraction& rhs) const;
    bool operator!=(const Fraction& rhs) const;
    bool operator<(const Fraction& rhs) const;
    bool operator<=(const Fraction& rhs) const;
    bool operator>(const Fraction& rhs) const;
    bool operator>=(const Fraction& rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const Fraction& f);
    friend std::istream& operator>>(std::istream& is, Fraction& f);

    Fraction powUnsignedInt(unsigned int exp) const;
    Fraction rootNatural(unsigned int n, const Fraction& eps) const;

    Fraction lnSmall(const Fraction& eps) const;
    Fraction ln(const Fraction& eps) const;
    Fraction log2(const Fraction& eps) const;
    Fraction log10(const Fraction& eps) const;

    Fraction sin(const Fraction& eps) const;
    Fraction cos(const Fraction& eps) const;
    Fraction tg(const Fraction& eps) const;
    Fraction ctg(const Fraction& eps) const;
    Fraction sec(const Fraction& eps) const;
    Fraction cosec(const Fraction& eps) const;

    Fraction arctg(const Fraction& eps) const;
    Fraction arcctg(const Fraction& eps) const;
    Fraction arcsin(const Fraction& eps) const;
    Fraction arccos(const Fraction& eps) const;
    Fraction arcsec(const Fraction& eps) const;
    Fraction arccosec(const Fraction& eps) const;
};

#endif
