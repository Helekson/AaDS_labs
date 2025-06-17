#ifndef MATRIX_H
#define MATRIX_H

#include <stdexcept>
#include <cmath>

class Matrix {
private:
    size_t rows;
    size_t cols;
    double** data;

    void allocateMemory();
    void freeMemory();

public:
    Matrix(size_t rows, size_t cols);
    Matrix(const Matrix& other);
    ~Matrix();
    Matrix& operator=(const Matrix& other);

    size_t getRows() const;
    size_t getCols() const;

    double* operator[](size_t row);
    const double* operator[](size_t row) const;

    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(double scalar) const;
    friend Matrix operator*(double scalar, const Matrix& matrix);

    Matrix transpose() const;
    double determinant(double epsilon = 1e-9) const;
    Matrix inverse(double epsilon = 1e-9) const;

    static bool compareMatrices(const Matrix& a, const Matrix& b, double epsilon = 1e-9);
    static double roundToZero(double value, double epsilon = 1e-9);
    static Matrix createIdentity(size_t size);
};

#endif 
