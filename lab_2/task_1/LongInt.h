#ifndef LONG_INT_H
#define LONG_INT_H

#include <iostream>
#include <stdexcept>

class LongInt {
private:
    int sign;
    unsigned int* digits;
    size_t size;
    static const unsigned long long BASE = 0x100000000ULL;  // base 2^32

    void resize(size_t newSize);
    void normalize();
    void complementToTwo();
    void addMagnitude(const unsigned int* otherDigits, size_t otherSize);
    void subtractMagnitude(const unsigned int* larger, size_t largeSize, const unsigned int* smaller, size_t smallSize);
    int compareMagnitude(const LongInt& other) const;
    unsigned int charToValue(char c) const;
    void fromString(const char* str, unsigned int base);
    void shiftLeftOne();
    void shiftRightOne();

    bool getBit(int pos) const;
    void setBit(int pos);
    void divide(const LongInt& divisor, LongInt& quotient, LongInt& remainder) const;

public:
    LongInt();
    LongInt(const LongInt& other);
    LongInt(int* digitsArr, size_t arrSize, int sign);
    LongInt(const char* str, unsigned int base);
    ~LongInt();

    LongInt& operator=(const LongInt& other);

    LongInt& operator+=(const LongInt& other);
    LongInt operator+(const LongInt& other) const;
    LongInt& operator-=(const LongInt& other);
    LongInt operator-(const LongInt& other) const;
    LongInt operator*(const LongInt& other) const;
    LongInt operator/(const LongInt& other) const;
    LongInt operator%(const LongInt& other) const;
    LongInt& operator++();
    LongInt operator++(int);
    LongInt& operator--();
    LongInt operator--(int);

    bool operator==(const LongInt& other) const;
    bool operator!=(const LongInt& other) const;
    bool operator<(const LongInt& other) const;
    bool operator<=(const LongInt& other) const;
    bool operator>(const LongInt& other) const;
    bool operator>=(const LongInt& other) const;

    LongInt operator~() const;
    LongInt& operator&=(const LongInt& other);
    LongInt operator&(const LongInt& other) const;
    LongInt& operator|=(const LongInt& other);
    LongInt operator|(const LongInt& other) const;
    LongInt& operator^=(const LongInt& other);
    LongInt operator^(const LongInt& other) const;

    LongInt& operator<<=(size_t shift);
    LongInt operator<<(size_t shift) const;
    LongInt& operator>>=(size_t shift);
    LongInt operator>>(size_t shift) const;

    friend std::ostream& operator<<(std::ostream& os, const LongInt& num);
    friend std::istream& operator>>(std::istream& is, LongInt& num);
};

#endif
