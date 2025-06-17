#include "LongInt.h"

// изменение размера
void LongInt::resize(size_t newSize) {
    unsigned int* newDigits = new unsigned int[newSize];
    size_t minSize = size < newSize ? size : newSize;
    for (size_t i = 0; i < minSize; ++i) newDigits[i] = digits[i];
    for (size_t i = minSize; i < newSize; ++i) newDigits[i] = 0;
    delete[] digits;
    digits = newDigits;
    size = newSize;
}

// уборка старших нулей
void LongInt::normalize() {
    while (size > 1 && digits[size - 1] == 0) --size;
    if (size == 0) {
        sign = 1;
        size = 1;
        digits[0] = 0;
    } else if (digits[0] == 0 && size == 1) {
        sign = 1;
    }
}

// в двоичный доп. код
void LongInt::complementToTwo() {
    if (sign == 1) return;
    for (size_t i = 0; i < size; ++i) digits[i] = ~digits[i];
    unsigned int carry = 1;
    for (size_t i = 0; carry && i < size; ++i) {
        unsigned long long sum = (unsigned long long)digits[i] + carry;
        digits[i] = sum & 0xFFFFFFFF;
        carry = sum >> 32;
    }
    sign = 1;
}

// сложение модулей
void LongInt::addMagnitude(const unsigned int* otherDigits, size_t otherSize) {
    size_t maxSize = size > otherSize ? size : otherSize;
    resize(maxSize + 1);
    unsigned int carry = 0;
    for (size_t i = 0; i < maxSize; ++i) {
        unsigned long long sum = (unsigned long long)digits[i] + (i < otherSize ? otherDigits[i] : 0) + carry;
        digits[i] = sum & 0xFFFFFFFF;
        carry = sum >> 32;
    }
    digits[maxSize] = carry;
    normalize();
}

// вычитание модулей
void LongInt::subtractMagnitude(const unsigned int* larger, size_t largeSize, const unsigned int* smaller, size_t smallSize) {
    resize(largeSize);
    unsigned int borrow = 0;
    for (size_t i = 0; i < largeSize; ++i) {
        unsigned long long diff = (unsigned long long)larger[i] - (i < smallSize ? smaller[i] : 0) - borrow;
        borrow = (diff >> 63) ? 1 : 0;
        digits[i] = diff & 0xFFFFFFFF;
    }
    normalize();
}

// сравнение модулей
int LongInt::compareMagnitude(const LongInt& other) const {
    if (size > other.size) return 1;
    if (size < other.size) return -1;
    for (int i = (int)size - 1; i >= 0; --i) {
        if (digits[i] > other.digits[i]) return 1;
        if (digits[i] < other.digits[i]) return -1;
    }
    return 0;
}

// символ→цифра
unsigned int LongInt::charToValue(char c) const {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

// разбор из строки
void LongInt::fromString(const char* str, unsigned int base) {
    size_t len = 0;
    while (str[len] != '\0') ++len;
    sign = 1;
    if (str[0] == '-') {
        sign = -1;
        ++str;
        --len;
    }
    size = 1;
    digits = new unsigned int[size]{0};
    for (size_t i = 0; i < len; ++i) {
        unsigned int digit = charToValue(str[i]);
        LongInt tempBase(reinterpret_cast<int*>(new unsigned int[1]{base}), 1, 1);
        *this = (*this) * tempBase;
        int* arr = new int[1];
        arr[0] = static_cast<int>(digit);
        LongInt tempDigit(arr, 1, 1);
        delete[] arr;
        *this += tempDigit;
    }
    if (sign < 0) this->sign = -1;
}

// сдвиг влево на 1 бит
void LongInt::shiftLeftOne() {
    unsigned long long carry = 0;
    for (size_t i = 0; i < size; ++i) {
        unsigned long long cur = ((unsigned long long)digits[i] << 1) | carry;
        digits[i] = cur & 0xFFFFFFFF;
        carry = cur >> 32;
    }
    if (carry) {
        resize(size + 1);
        digits[size - 1] = (unsigned int)carry;
    }
    normalize();
}

// сдвиг вправо на 1 бит
void LongInt::shiftRightOne() {
    unsigned long long carry = 0;
    for (int i = (int)size - 1; i >= 0; --i) {
        unsigned long long cur = ((unsigned long long)carry << 32) | digits[i];
        digits[i] = (unsigned int)(cur >> 1);
        carry = cur & 1;
    }
    normalize();
}

// получить i-й бит
bool LongInt::getBit(int pos) const {
    if (pos < 0) return false;
    int digitIndex = pos / 32;
    int bitIndex = pos % 32;
    if ((size_t)digitIndex >= size) return false;
    return (digits[digitIndex] >> bitIndex) & 1U;
}

// установить i-й бит
void LongInt::setBit(int pos) {
    if (pos < 0) return;
    int digitIndex = pos / 32;
    int bitIndex = pos % 32;
    if ((size_t)digitIndex >= size) {
        resize(digitIndex + 1);
    }
    digits[digitIndex] |= (1U << bitIndex);
}

// деление
void LongInt::divide(const LongInt& divisor, LongInt& quotient, LongInt& remainder) const {
    if (divisor == LongInt()) {
        throw std::runtime_error("Division by zero in LongInt::divide()");
    }
    LongInt u = *this;
    LongInt v = divisor;
    if (u.sign < 0) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        u = u * minusOne;
    }
    if (v.sign < 0) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        v = v * minusOne;
    }
    quotient = LongInt();
    remainder = LongInt();
    quotient.resize(u.size);
    for (size_t i = 0; i < quotient.size; ++i) quotient.digits[i] = 0;
    quotient.sign = this->sign * divisor.sign;
    remainder = LongInt(reinterpret_cast<int*>(new unsigned int[1]{0}), 1, 1);
    int totalBits = (int)u.size * 32;
    for (int i = totalBits - 1; i >= 0; --i) {
        remainder.shiftLeftOne();
        if (u.getBit(i)) {
            remainder.digits[0] |= 1U;
        }
        remainder.normalize();
        if (remainder.compareMagnitude(v) >= 0) {
            remainder.subtractMagnitude(remainder.digits, remainder.size, v.digits, v.size);
            quotient.setBit(i);
        }
    }
    quotient.normalize();
    remainder.sign = this->sign;
    remainder.normalize();
}

// конструктор по умолчанию
LongInt::LongInt() : sign(1), size(1), digits(new unsigned int[1]{0}) {}

// копирующий
LongInt::LongInt(const LongInt& other)
    : sign(other.sign), size(other.size), digits(new unsigned int[other.size]) {
    for (size_t i = 0; i < size; ++i) digits[i] = other.digits[i];
}

// из массива
LongInt::LongInt(int* digitsArr, size_t arrSize, int sgn)
    : sign(sgn), size(arrSize), digits(new unsigned int[arrSize]) {
    for (size_t i = 0; i < arrSize; ++i) {
        digits[i] = static_cast<unsigned int>(digitsArr[i]);
    }
    normalize();
}

// из строки
LongInt::LongInt(const char* str, unsigned int base) : sign(1), size(1), digits(new unsigned int[1]{0}) {
    fromString(str, base);
}

// деструктор
LongInt::~LongInt() {
    delete[] digits;
}

// присваивание
LongInt& LongInt::operator=(const LongInt& other) {
    if (this != &other) {
        delete[] digits;
        sign = other.sign;
        size = other.size;
        digits = new unsigned int[size];
        for (size_t i = 0; i < size; ++i) digits[i] = other.digits[i];
    }
    return *this;
}

// +=
LongInt& LongInt::operator+=(const LongInt& other) {
    if (sign == other.sign) {
        addMagnitude(other.digits, other.size);
    } else {
        int cmp = compareMagnitude(other);
        if (cmp == 0) {
            *this = LongInt();
        } else if (cmp > 0) {
            subtractMagnitude(digits, size, other.digits, other.size);
        } else {
            LongInt temp = other;
            temp.subtractMagnitude(temp.digits, temp.size, digits, size);
            *this = temp;
            sign = other.sign;
        }
    }
    return *this;
}

// +
LongInt LongInt::operator+(const LongInt& other) const {
    LongInt result(*this);
    result += other;
    return result;
}

// -=
LongInt& LongInt::operator-=(const LongInt& other) {
    LongInt temp = other;
    temp.sign = -temp.sign;
    *this += temp;
    return *this;
}

// -
LongInt LongInt::operator-(const LongInt& other) const {
    LongInt result(*this);
    result -= other;
    return result;
}

// *
LongInt LongInt::operator*(const LongInt& other) const {
    LongInt result;
    result.resize(size + other.size);
    for (size_t i = 0; i < size; ++i) {
        unsigned int carry = 0;
        for (size_t j = 0; j < other.size || carry; ++j) {
            unsigned long long prod = result.digits[i + j]
                + (unsigned long long)digits[i] * (j < other.size ? other.digits[j] : 0)
                + carry;
            result.digits[i + j] = static_cast<unsigned int>(prod & 0xFFFFFFFF);
            carry = static_cast<unsigned int>(prod >> 32);
        }
    }
    result.sign = sign * other.sign;
    result.normalize();
    return result;
}

// /
LongInt LongInt::operator/(const LongInt& other) const {
    LongInt q, r;
    divide(other, q, r);
    return q;
}

// %
LongInt LongInt::operator%(const LongInt& other) const {
    LongInt q, r;
    divide(other, q, r);
    return r;
}

// префикс ++
LongInt& LongInt::operator++() {
    LongInt one(reinterpret_cast<int*>(new unsigned int[1]{1}), 1, 1);
    *this += one;
    return *this;
}

// постфикс ++
LongInt LongInt::operator++(int) {
    LongInt tmp(*this);
    ++(*this);
    return tmp;
}

// префикс --
LongInt& LongInt::operator--() {
    LongInt one(reinterpret_cast<int*>(new unsigned int[1]{1}), 1, 1);
    *this -= one;
    return *this;
}

// постфикс --
LongInt LongInt::operator--(int) {
    LongInt tmp(*this);
    --(*this);
    return tmp;
}

// ==
bool LongInt::operator==(const LongInt& other) const {
    if (sign != other.sign) return false;
    if (size != other.size) return false;
    for (size_t i = 0; i < size; ++i) {
        if (digits[i] != other.digits[i]) return false;
    }
    return true;
}

// !=
bool LongInt::operator!=(const LongInt& other) const {
    return !(*this == other);
}

// <
bool LongInt::operator<(const LongInt& other) const {
    if (sign != other.sign) return sign < other.sign;
    int cmp = compareMagnitude(other);
    if (sign == 1) return (cmp < 0);
    else return (cmp > 0);
}

// <=
bool LongInt::operator<=(const LongInt& other) const {
    return (*this < other) || (*this == other);
}

// >
bool LongInt::operator>(const LongInt& other) const {
    return !(*this <= other);
}

// >=
bool LongInt::operator>=(const LongInt& other) const {
    return !(*this < other);
}

// побит. отрицание
LongInt LongInt::operator~() const {
    LongInt result(*this);
    for (size_t i = 0; i < result.size; ++i) {
        result.digits[i] = ~result.digits[i];
    }
    result.normalize();
    return result;
}

// &=
LongInt& LongInt::operator&=(const LongInt& other) {
    *this = *this & other;
    return *this;
}

// &
LongInt LongInt::operator&(const LongInt& other) const {
    LongInt result;
    size_t maxSize = size > other.size ? size : other.size;
    result.resize(maxSize);
    for (size_t i = 0; i < maxSize; ++i) {
        unsigned int a = (i < size) ? digits[i] : (sign < 0 ? 0xFFFFFFFF : 0);
        unsigned int b = (i < other.size) ? other.digits[i] : (other.sign < 0 ? 0xFFFFFFFF : 0);
        result.digits[i] = a & b;
    }
    result.normalize();
    return result;
}

// |=
LongInt& LongInt::operator|=(const LongInt& other) {
    *this = *this | other;
    return *this;
}

// |
LongInt LongInt::operator|(const LongInt& other) const {
    LongInt result;
    size_t maxSize = size > other.size ? size : other.size;
    result.resize(maxSize);
    for (size_t i = 0; i < maxSize; ++i) {
        unsigned int a = (i < size) ? digits[i] : (sign < 0 ? 0xFFFFFFFF : 0);
        unsigned int b = (i < other.size) ? other.digits[i] : (other.sign < 0 ? 0xFFFFFFFF : 0);
        result.digits[i] = a | b;
    }
    result.normalize();
    return result;
}

// ^=
LongInt& LongInt::operator^=(const LongInt& other) {
    *this = *this ^ other;
    return *this;
}

// ^
LongInt LongInt::operator^(const LongInt& other) const {
    LongInt result;
    size_t maxSize = size > other.size ? size : other.size;
    result.resize(maxSize);
    for (size_t i = 0; i < maxSize; ++i) {
        unsigned int a = (i < size) ? digits[i] : (sign < 0 ? 0xFFFFFFFF : 0);
        unsigned int b = (i < other.size) ? other.digits[i] : (other.sign < 0 ? 0xFFFFFFFF : 0);
        result.digits[i] = a ^ b;
    }
    result.normalize();
    return result;
}

// сдвиг влево
LongInt& LongInt::operator<<=(size_t shift) {
    for (size_t i = 0; i < shift; ++i) {
        shiftLeftOne();
    }
    return *this;
}

// <<
LongInt LongInt::operator<<(size_t shift) const {
    LongInt result(*this);
    result <<= shift;
    return result;
}

// сдвиг вправо
LongInt& LongInt::operator>>=(size_t shift) {
    for (size_t i = 0; i < shift; ++i) {
        shiftRightOne();
    }
    return *this;
}

// >>
LongInt LongInt::operator>>(size_t shift) const {
    LongInt result(*this);
    result >>= shift;
    return result;
}

// вывод
std::ostream& operator<<(std::ostream& os, const LongInt& num) {
    if (num.sign < 0) os << '-';
    for (int i = (int)num.size - 1; i >= 0; --i) {
        os << num.digits[i];
    }
    return os;
}

// ввод
std::istream& operator>>(std::istream& is, LongInt& num) {
    char buffer[1024];
    is >> buffer;
    num = LongInt(buffer, 10);
    return is;
}
