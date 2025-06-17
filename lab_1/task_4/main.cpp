#include "matrix.h"
#include <iostream>

void print2x2Matrix(const Matrix& m) {
    const double epsilon = 1e-12;
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            double value = Matrix::roundToZero(m[i][j], epsilon);
            std::cout << value << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "------------------------\n";
}

void printMatrix(const Matrix& m) {
    const double epsilon = 1e-12;
    for (size_t i = 0; i < m.getRows(); ++i) {
        for (size_t j = 0; j < m.getCols(); ++j) {
            double value = Matrix::roundToZero(m[i][j], epsilon);
            std::cout << value << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "------------------------\n";
}

void test5x5() {
    try {
        // Тест 1: Обратная единичной матрицы
        Matrix identity = Matrix::createIdentity(5);
        Matrix invIdentity = identity.inverse();
        std::cout << "Тест 1: Обратная единичной матрицы:\n";
        printMatrix(invIdentity);
        bool test1 = Matrix::compareMatrices(identity, invIdentity);
        std::cout << (test1 ? "OK" : "FAIL") << "\n\n";

        // Тест 2: Определитель верхней треугольной матрицы
        Matrix upperTri(5, 5);
        for (size_t i = 0; i < 5; ++i) {
            for (size_t j = i; j < 5; ++j) {
                upperTri[i][j] = i + j + 1;
            }
        }
        double det = upperTri.determinant();
        std::cout << "Тест 2: Определитель = " << det << "\n";
        std::cout << "Ожидалось: 1 * 3 * 5 * 7 * 9 = 945\n";
        std::cout << (std::abs(det - 945.0) < 1e-6 ? "OK" : "FAIL") << "\n\n";

        // Тест 3: Проверка A * A^{-1} = I
        Matrix randomMatrix(5, 5);
        // Заполнение матрицы невырожденными значениями
        for (size_t i = 0; i < 5; ++i) {
            for (size_t j = 0; j < 5; ++j) {
                randomMatrix[i][j] = (i == j) ? (i + 1) * 2.0 : 0.1;
            }
        }
        Matrix invRandom = randomMatrix.inverse();
        Matrix product = randomMatrix * invRandom;
        Matrix identityCheck = Matrix::createIdentity(5);
        std::cout << "Тест 3: Результат A * A^{-1}:\n";
        printMatrix(product);
        bool test3 = Matrix::compareMatrices(product, identityCheck, 1e-9);
        std::cout << (test3 ? "OK" : "FAIL") << "\n\n";
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}

void test2x2() {
    try {
        Matrix m1(2, 2);
        m1[0][0] = 1; m1[0][1] = 2;
        m1[1][0] = 3; m1[1][1] = 4;

        Matrix m2(2, 2);
        m2[0][0] = 5; m2[0][1] = 6;
        m2[1][0] = 7; m2[1][1] = 8;

        Matrix sum = m1 + m2;
        Matrix product = m1 * m2;
        Matrix transposed = m1.transpose();
        double det = m1.determinant();
        Matrix inverse = m1.inverse();

        std::cout << "=== Тесты для матриц 2x2 ===\n";
        std::cout << "Матрица 1:\n"; print2x2Matrix(m1);
        std::cout << "Матрица 2:\n"; print2x2Matrix(m2);
        std::cout << "Сумма:\n"; print2x2Matrix(sum);
        std::cout << "Произведение:\n"; print2x2Matrix(product);
        std::cout << "Транспонированная:\n"; print2x2Matrix(transposed);
        std::cout << "Определитель: " << det << "\n";
        std::cout << "Обратная матрица:\n"; print2x2Matrix(inverse);

        Matrix identity = m1 * inverse;
        std::cout << "A * A^{-1}:\n"; print2x2Matrix(identity);
        bool testIdentity = Matrix::compareMatrices(identity, Matrix(2, 2));
        std::cout << (testIdentity ? "OK" : "FAIL") << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}

int main() {
    test2x2(); 
    test5x5(); 
    return 0;
}
