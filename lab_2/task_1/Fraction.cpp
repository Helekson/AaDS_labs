#include "Fraction.h"

// absLI
LongInt Fraction::absLI(const LongInt& x) {
    LongInt tmp = x;
    if (tmp < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        tmp = tmp * minusOne;
    }
    return tmp;
}

// gcdLI
LongInt Fraction::gcdLI(LongInt a, LongInt b) {
    if (a < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        a = a * minusOne;
    }
    if (b < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        b = b * minusOne;
    }
    while (!(b == LongInt())) {
        LongInt r = a % b;
        a = b;
        b = r;
    }
    return a;
}

// reduce
void Fraction::reduce() {
    if (denominator == LongInt()) {
        throw std::runtime_error("Ошибка: деление на ноль в Fraction::reduce()");
    }
    if (numerator < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        numerator = numerator * minusOne;
        denominator = denominator * minusOne;
    }
    LongInt anum = absLI(numerator);
    LongInt aden = absLI(denominator);
    LongInt G = gcdLI(anum, aden);
    if (!(G == LongInt(reinterpret_cast<int*>(new unsigned int[1]{1}), 1, 1))) {
        numerator = anum / G;
        denominator = aden / G;
        if (denominator < LongInt()) {
            LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
            denominator = denominator * minusOne;
        }
    }
}

// makeLongFromULL
LongInt Fraction::makeLongFromULL(unsigned long long v) {
    if (v == 0ULL) {
        return LongInt(reinterpret_cast<int*>(new unsigned int[1]{0}), 1, 1);
    }
    unsigned long long lo = v & 0xFFFFFFFFULL;
    unsigned long long hi = (v >> 32) & 0xFFFFFFFFULL;
    if (hi == 0ULL) {
        int* arr = new int[1];
        arr[0] = static_cast<int>(lo);
        LongInt R(arr, 1, 1);
        delete[] arr;
        return R;
    } else {
        int* arr = new int[2];
        arr[0] = static_cast<int>(lo);
        arr[1] = static_cast<int>(hi);
        LongInt R(arr, 2, 1);
        delete[] arr;
        return R;
    }
}

// конструкторы и оператор=
Fraction::Fraction()
    : numerator(reinterpret_cast<int*>(new unsigned int[1]{0}), 1, 1),
      denominator(reinterpret_cast<int*>(new unsigned int[1]{1}), 1, 1)
{}

Fraction::Fraction(const LongInt& num, const LongInt& den) {
    numerator = num;
    denominator = den;
    reduce();
}

Fraction::Fraction(int v) {
    int* arr = new int[1];
    arr[0] = v < 0 ? -v : v;
    LongInt L(arr, 1, (v < 0 ? -1 : 1));
    delete[] arr;
    numerator = absLI(L);
    denominator = makeLongFromULL(1ULL);
    if (L < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        denominator = makeLongFromULL(1ULL) * minusOne;
    }
    reduce();
}

Fraction::Fraction(const Fraction& other)
    : numerator(other.numerator), denominator(other.denominator)
{}

Fraction& Fraction::operator=(const Fraction& other) {
    if (this == &other) return *this;
    numerator = other.numerator;
    denominator = other.denominator;
    return *this;
}

// +=
Fraction& Fraction::operator+=(const Fraction& rhs) {
    LongInt A = absLI(numerator);
    LongInt B = absLI(denominator);
    if (denominator < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        A = A * minusOne;
    }
    LongInt C = absLI(rhs.numerator);
    LongInt D = absLI(rhs.denominator);
    if (rhs.denominator < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        C = C * minusOne;
    }
    LongInt newNumer = A * D + C * B;
    LongInt newDenom = B * D;
    if (newNumer < LongInt()) {
        newNumer = absLI(newNumer);
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        newDenom = newDenom * minusOne;
    }
    numerator = newNumer;
    denominator = newDenom;
    reduce();
    return *this;
}

// +
Fraction Fraction::operator+(const Fraction& rhs) const {
    Fraction tmp(*this);
    tmp += rhs;
    return tmp;
}

// -=
Fraction& Fraction::operator-=(const Fraction& rhs) {
    Fraction negR = rhs;
    LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
    negR.denominator = negR.denominator * minusOne;
    return (*this) += negR;
}

// -
Fraction Fraction::operator-(const Fraction& rhs) const {
    Fraction tmp(*this);
    tmp -= rhs;
    return tmp;
}

// *=
Fraction& Fraction::operator*=(const Fraction& rhs) {
    LongInt A = absLI(numerator);
    LongInt B = absLI(denominator);
    if (denominator < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        A = A * minusOne;
    }
    LongInt C = absLI(rhs.numerator);
    LongInt D = absLI(rhs.denominator);
    if (rhs.denominator < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        C = C * minusOne;
    }
    LongInt newNumer = A * C;
    LongInt newDenom = B * D;
    if (newNumer < LongInt()) {
        newNumer = absLI(newNumer);
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        newDenom = newDenom * minusOne;
    }
    numerator = newNumer;
    denominator = newDenom;
    reduce();
    return *this;
}

// *
Fraction Fraction::operator*(const Fraction& rhs) const {
    Fraction tmp(*this);
    tmp *= rhs;
    return tmp;
}

// /=
Fraction& Fraction::operator/=(const Fraction& rhs) {
    if (rhs.numerator == LongInt()) {
        throw std::runtime_error("Ошибка: деление на ноль в Fraction::operator/=");
    }
    LongInt C = absLI(rhs.numerator);
    LongInt D = absLI(rhs.denominator);
    if (rhs.denominator < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        C = C * minusOne;
    }
    LongInt A = absLI(numerator);
    LongInt B = absLI(denominator);
    if (denominator < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        A = A * minusOne;
    }
    LongInt newNumer = A * D;
    LongInt newDenom = B * C;
    if (newNumer < LongInt()) {
        newNumer = absLI(newNumer);
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        newDenom = newDenom * minusOne;
    }
    numerator = newNumer;
    denominator = newDenom;
    reduce();
    return *this;
}

// /
Fraction Fraction::operator/(const Fraction& rhs) const {
    Fraction tmp(*this);
    tmp /= rhs;
    return tmp;
}

// ++
Fraction& Fraction::operator++() {
    (*this) += Fraction(1);
    return *this;
}

Fraction Fraction::operator++(int) {
    Fraction old = *this;
    ++(*this);
    return old;
}

// --
Fraction& Fraction::operator--() {
    (*this) -= Fraction(1);
    return *this;
}

Fraction Fraction::operator--(int) {
    Fraction old = *this;
    --(*this);
    return old;
}

// ==
bool Fraction::operator==(const Fraction& rhs) const {
    return (numerator == rhs.numerator) && (denominator == rhs.denominator);
}

// !=
bool Fraction::operator!=(const Fraction& rhs) const {
    return !(*this == rhs);
}

// <
bool Fraction::operator<(const Fraction& rhs) const {
    LongInt A = absLI(numerator);
    LongInt B = absLI(denominator);
    if (denominator < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        A = A * minusOne;
    }
    LongInt C = absLI(rhs.numerator);
    LongInt D = absLI(rhs.denominator);
    if (rhs.denominator < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        C = C * minusOne;
    }
    LongInt left = A * D;
    LongInt right = C * B;
    return left < right;
}

// <=
bool Fraction::operator<=(const Fraction& rhs) const {
    return (*this < rhs) || (*this == rhs);
}

// >
bool Fraction::operator>(const Fraction& rhs) const {
    return !(*this <= rhs);
}

// >=
bool Fraction::operator>=(const Fraction& rhs) const {
    return !(*this < rhs);
}

// <<
std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    if (f.denominator < LongInt()) {
        os << '-';
    }
    os << f.numerator;
    os << '/';
    LongInt absDen = Fraction::absLI(f.denominator);
    os << absDen;
    return os;
}

// >>
std::istream& operator>>(std::istream& is, Fraction& f) {
    char buf[2048];
    is >> buf;
    int slash_pos = -1;
    for (int i = 0; buf[i] != '\0'; ++i) {
        if (buf[i] == '/') {
            slash_pos = i;
            break;
        }
    }
    if (slash_pos < 0) {
        throw std::runtime_error("Неверный формат дроби: не найден '/'");
    }
    char numbuf[2048];
    char denbuf[2048];
    int i = 0;
    for (; i < slash_pos; ++i) {
        numbuf[i] = buf[i];
    }
    numbuf[i] = '\0';
    int j = 0;
    for (i = slash_pos + 1; buf[i] != '\0'; ++i, ++j) {
        denbuf[j] = buf[i];
    }
    denbuf[j] = '\0';
    LongInt nVal = LongInt(numbuf, 10);
    LongInt dVal = LongInt(denbuf, 10);
    if (dVal == LongInt()) {
        throw std::runtime_error("Ошибка: деление на ноль при вводе дроби");
    }
    f.numerator = nVal;
    f.denominator = dVal;
    f.reduce();
    return is;
}

// powUnsignedInt
Fraction Fraction::powUnsignedInt(unsigned int exp) const {
    Fraction result(1);
    Fraction base = *this;
    unsigned int e = exp;
    while (e > 0) {
        if (e & 1U) {
            result *= base;
        }
        base *= base;
        e >>= 1;
    }
    return result;
}

// rootNatural
Fraction Fraction::rootNatural(unsigned int n, const Fraction& eps) const {
    if (n == 0) {
        throw std::runtime_error("Ошибка: корень нулевой степени");
    }
    if (numerator == LongInt()) {
        return Fraction(0);
    }
    Fraction x = *this;
    if (x < Fraction(0)) {
        throw std::runtime_error("Ошибка: отрицательное основание для корня");
    }
    Fraction y(1);
    while (true) {
        Fraction y_pow_n_minus1 = y.powUnsignedInt(n - 1);
        Fraction temp = x / y_pow_n_minus1;
        LongInt coef = makeLongFromULL(static_cast<unsigned long long>(n - 1));
        Fraction numeratorPart = y * Fraction(coef, makeLongFromULL(1ULL));
        Fraction sum = numeratorPart + temp;
        LongInt denomInt = makeLongFromULL(static_cast<unsigned long long>(n));
        Fraction newY = sum / Fraction(denomInt, makeLongFromULL(1ULL));
        Fraction diff = newY - y;
        Fraction absDiff = diff;
        if (absDiff.denominator < LongInt()) {
            LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
            absDiff.denominator = absDiff.denominator * minusOne;
        }
        if (absDiff <= eps) {
            return newY;
        }
        y = newY;
    }
}

// lnSmall
Fraction Fraction::lnSmall(const Fraction& eps) const {
    Fraction one(1);
    Fraction x = *this;
    Fraction t = (x - one) / (x + one);
    Fraction t_pow = t;
    Fraction sum = t;
    unsigned int k = 1;
    while (true) {
        Fraction t2 = t * t;
        t_pow = t_pow * t2;
        unsigned long long denomIntULL = static_cast<unsigned long long>(2 * k + 1);
        LongInt denomLong = makeLongFromULL(denomIntULL);
        Fraction term = t_pow / Fraction(denomLong, makeLongFromULL(1ULL));
        sum += term;
        Fraction absTerm = term;
        if (absTerm.denominator < LongInt()) {
            LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
            absTerm.denominator = absTerm.denominator * minusOne;
        }
        if (absTerm <= eps) break;
        ++k;
    }
    return sum * Fraction(makeLongFromULL(2ULL), makeLongFromULL(1ULL));
}

// ln
Fraction Fraction::ln(const Fraction& eps) const {
    if (*this <= Fraction(0)) {
        throw std::runtime_error("Ошибка: ln определён только для положительных x");
    }
    Fraction one(1);
    Fraction two(2);
    Fraction x = *this;
    int shiftCount = 0;
    while (x > two) {
        x /= two;
        ++shiftCount;
    }
    while (x <= one) {
        x *= two;
        --shiftCount;
    }
    Fraction baseLn = x.lnSmall(eps);
    Fraction ln2 = two.lnSmall(eps);
    Fraction shiftPart = ln2 * Fraction(makeLongFromULL(static_cast<unsigned long long>(shiftCount)), makeLongFromULL(1ULL));
    return baseLn + shiftPart;
}

// log2
Fraction Fraction::log2(const Fraction& eps) const {
    Fraction lx = this->ln(eps);
    Fraction two(2);
    Fraction ln2 = two.lnSmall(eps);
    return lx / ln2;
}

// log10
Fraction Fraction::log10(const Fraction& eps) const {
    if (*this <= Fraction(0)) {
        throw std::runtime_error("Ошибка: log10 определён только для положительных x");
    }
    Fraction lx = this->ln(eps);
    Fraction ten(makeLongFromULL(10ULL), makeLongFromULL(1ULL));
    Fraction ln10 = ten.lnSmall(eps);
    return lx / ln10;
}

// sin
Fraction Fraction::sin(const Fraction& eps) const {
    Fraction x = *this;
    Fraction x2 = x * x;
    Fraction term = x;
    Fraction sum = term;
    unsigned int n = 0;
    while (true) {
        unsigned long long a = static_cast<unsigned long long>(2 * n + 2);
        unsigned long long b = static_cast<unsigned long long>(2 * n + 3);
        unsigned long long cULL = a * b;
        LongInt cLong = makeLongFromULL(cULL);
        Fraction nume = term * x2;
        nume.denominator = nume.denominator * LongInt(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        Fraction nextTerm = nume / Fraction(cLong, makeLongFromULL(1ULL));
        sum += nextTerm;
        Fraction absNext = nextTerm;
        if (absNext.denominator < LongInt()) {
            LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
            absNext.denominator = absNext.denominator * minusOne;
        }
        if (absNext <= eps) break;
        term = nextTerm;
        ++n;
    }
    return sum;
}

// cos
Fraction Fraction::cos(const Fraction& eps) const {
    Fraction x = *this;
    Fraction x2 = x * x;
    Fraction term(1);
    Fraction sum = term;
    unsigned int n = 0;
    while (true) {
        unsigned long long a = static_cast<unsigned long long>(2 * n + 1);
        unsigned long long b = static_cast<unsigned long long>(2 * n + 2);
        unsigned long long cULL = a * b;
        LongInt cLong = makeLongFromULL(cULL);
        Fraction nume = term * x2;
        nume.denominator = nume.denominator * LongInt(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        Fraction nextTerm = nume / Fraction(cLong, makeLongFromULL(1ULL));
        sum += nextTerm;
        Fraction absNext = nextTerm;
        if (absNext.denominator < LongInt()) {
            LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
            absNext.denominator = absNext.denominator * minusOne;
        }
        if (absNext <= eps) break;
        term = nextTerm;
        ++n;
    }
    return sum;
}

// tg
Fraction Fraction::tg(const Fraction& eps) const {
    Fraction s = this->sin(eps);
    Fraction c = this->cos(eps);
    if (c == Fraction(0)) {
        throw std::runtime_error("Ошибка: tan не определён, cos(x)=0");
    }
    return s / c;
}

// ctg
Fraction Fraction::ctg(const Fraction& eps) const {
    Fraction s = this->sin(eps);
    if (s == Fraction(0)) {
        throw std::runtime_error("Ошибка: cot не определён, sin(x)=0");
    }
    Fraction c = this->cos(eps);
    return c / s;
}

// sec
Fraction Fraction::sec(const Fraction& eps) const {
    Fraction c = this->cos(eps);
    if (c == Fraction(0)) {
        throw std::runtime_error("Ошибка: sec не определён, cos(x)=0");
    }
    return Fraction(1) / c;
}

// cosec
Fraction Fraction::cosec(const Fraction& eps) const {
    Fraction s = this->sin(eps);
    if (s == Fraction(0)) {
        throw std::runtime_error("Ошибка: csc не определён, sin(x)=0");
    }
    return Fraction(1) / s;
}

// arctg
Fraction Fraction::arctg(const Fraction& eps) const {
    Fraction x = *this;
    Fraction absX = x;
    if (absX.denominator < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        absX.denominator = absX.denominator * minusOne;
    }
    if (absX > Fraction(1)) {
        throw std::runtime_error("Ошибка: arctg не поддерживается для |x|>1");
    }
    Fraction term = x;
    Fraction sum = term;
    unsigned int n = 0;
    while (true) {
        unsigned long long a = static_cast<unsigned long long>(2 * n + 1);
        unsigned long long b = static_cast<unsigned long long>(2 * n + 3);
        LongInt aLong = makeLongFromULL(a);
        LongInt bLong = makeLongFromULL(b);
        Fraction nume = term * (x * x) * Fraction(aLong, makeLongFromULL(1ULL));
        nume.denominator = nume.denominator * bLong;
        nume.denominator = nume.denominator * LongInt(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        Fraction nextTerm = nume;
        sum += nextTerm;
        Fraction absNext = nextTerm;
        if (absNext.denominator < LongInt()) {
            LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
            absNext.denominator = absNext.denominator * minusOne;
        }
        if (absNext <= eps) break;
        term = nextTerm;
        ++n;
    }
    return sum;
}

// arcctg
Fraction Fraction::arcctg(const Fraction& /*eps*/) const {
    throw std::runtime_error("Ошибочка: arcctg не реализован без константы PI");
}

// arcsin
Fraction Fraction::arcsin(const Fraction& eps) const {
    Fraction x = *this;
    Fraction absX = x;
    if (absX.denominator < LongInt()) {
        LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
        absX.denominator = absX.denominator * minusOne;
    }
    if (absX > Fraction(1)) {
        throw std::runtime_error("Ошибка: arcsin не определён для |x|>1");
    }
    Fraction term = x;
    Fraction sum = term;
    unsigned int n = 0;
    LongInt fact2n = makeLongFromULL(1ULL);
    LongInt factn = makeLongFromULL(1ULL);
    while (true) {
        unsigned long long two_n = static_cast<unsigned long long>(2 * n);
        unsigned long long two_n1 = two_n + 1ULL;
        unsigned long long two_n2 = two_n + 2ULL;
        fact2n = fact2n * makeLongFromULL(two_n1);
        fact2n = fact2n * makeLongFromULL(two_n2);
        factn = factn * makeLongFromULL(static_cast<unsigned long long>(n + 1ULL));
        ++n;
        LongInt twoNplus1 = makeLongFromULL(static_cast<unsigned long long>(2 * n + 1ULL));
        LongInt four = makeLongFromULL(4ULL);
        Fraction fourPow = Fraction(makeLongFromULL(1ULL), makeLongFromULL(1ULL));
        for (unsigned int i = 0; i < n; ++i) {
            fourPow *= Fraction(four, makeLongFromULL(1ULL));
        }
        Fraction factnSq = Fraction(factn, makeLongFromULL(1ULL));
        factnSq *= factnSq;
        Fraction denomCoef = fourPow * factnSq * Fraction(twoNplus1, makeLongFromULL(1ULL));
        Fraction coef = Fraction(fact2n, makeLongFromULL(1ULL)) / denomCoef;
        Fraction x2 = x * x;
        term = term * x2;
        Fraction nextTerm = coef * term;
        sum += nextTerm;
        Fraction absNext = nextTerm;
        if (absNext.denominator < LongInt()) {
            LongInt minusOne(reinterpret_cast<int*>(new unsigned int[1]{static_cast<unsigned int>(-1)}), 1, -1);
            absNext.denominator = absNext.denominator * minusOne;
        }
        if (absNext <= eps) break;
    }
    return sum;
}

// arccos
Fraction Fraction::arccos(const Fraction& /*eps*/) const {
    throw std::runtime_error("Ошибок: arccos не реализован без константы PI");
}

// arcsec
Fraction Fraction::arcsec(const Fraction& /*eps*/) const {
    throw std::runtime_error("Ошибка: arcsec не реализован без константы PI");
}

// arccosec
Fraction Fraction::arccosec(const Fraction& /*eps*/) const {
    throw std::runtime_error("Ошибка: arccosec не реализован без константы PI");
}
