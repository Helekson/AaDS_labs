#include "matrix.h"


size_t Matrix::getRows() const {
    return rows;
}

size_t Matrix::getCols() const {
    return cols;
}

//иначе не получим ноль при некоторых операциях
double Matrix::roundToZero(double value, double epsilon) {
    return (std::abs(value) < epsilon) ? 0.0 : value;
}

//выделяем память под новую матрицу
void Matrix::allocateMemory() {
    data = new double*[rows];
    for (size_t i = 0; i < rows; ++i) {
        data[i] = new double[cols](); 
    }
}

//освобождаем память
void Matrix::freeMemory() {
    for (size_t i = 0; i < rows; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

//конструктор с переданными столбцами и строками
Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
    if (rows == 0 || cols == 0) throw std::invalid_argument("Invalid matrix size");
    allocateMemory();
}


//конструктор с переданной матрицей
Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
    allocateMemory();
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) { 
            data[i][j] = other.data[i][j];
        }
    }
}

//деструктор
Matrix::~Matrix() {
    freeMemory();
}

//переопределение равно
Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        freeMemory();
        rows = other.rows;
        cols = other.cols;
        allocateMemory();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) { // Замена std::copy
                data[i][j] = other.data[i][j];
            }
        }
    }
    return *this;
}

//переопределение возврат элемента по индексу (строка)
//этот метод работает с неконстантными объектами матрицы (те можем поменять элемент)
//возращает указатель на изменяемый объект
double* Matrix::operator[](size_t row) {
    if (row >= rows) throw std::out_of_range("Row index out of range");
    return data[row];
}


//возвращает указатель на изменяемый объект(const double*)
const double* Matrix::operator[](size_t row) const {
    if (row >= rows) throw std::out_of_range("Row index out of range");
    return data[row];
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrices sizes mismatch");
    }
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrices sizes mismatch");
    }
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Matrices sizes mismatch");
    }
    Matrix result(rows, other.cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t k = 0; k < cols; ++k) {
            for (size_t j = 0; j < other.cols; ++j) {
                result[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = data[i][j] * scalar;
        }
    }
    return result;
}

Matrix operator*(double scalar, const Matrix& matrix) {
    return matrix * scalar;
}

Matrix Matrix::transpose() const {
    Matrix result(cols, rows);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[j][i] = data[i][j];
        }
    }
    return result;
}

bool Matrix::compareMatrices(const Matrix& a, const Matrix& b, double epsilon) {
    if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) return false;
    for (size_t i = 0; i < a.getRows(); ++i) {
        for (size_t j = 0; j < a.getCols(); ++j) {
            if (std::abs(a[i][j] - b[i][j]) > epsilon) return false;
        }
    }
    return true;
}

double Matrix::determinant(double epsilon) const {
    if (rows != cols) throw std::logic_error("Determinant is defined for square matrices");
    Matrix temp(*this);
    double det = 1.0;
    for (size_t i = 0; i < rows; ++i) {
        size_t pivot = i;
        for (size_t j = i + 1; j < rows; ++j) {
            if (std::abs(temp[j][i]) > std::abs(temp[pivot][i])) {
                pivot = j;
            }
        }
        if (std::abs(temp[pivot][i]) < epsilon) return 0;
        if (pivot != i) {
            double* tmp = temp.data[i];
            temp.data[i] = temp.data[pivot];
            temp.data[pivot] = tmp;
            det *= -1;
        }
        det *= temp[i][i];
        for (size_t j = i + 1; j < rows; ++j) {
            double factor = temp[j][i] / temp[i][i];
            for (size_t k = i; k < rows; ++k) {
                temp[j][k] -= factor * temp[i][k];
            }
        }
    }
    return Matrix::roundToZero(det, epsilon); //округляем до 0 если у нас околонулевое значение
}

Matrix Matrix::createIdentity(size_t size) {
    Matrix identity(size, size);
    for (size_t i = 0; i < size; ++i) {
        identity[i][i] = 1.0;
    }
    return identity;
}

Matrix Matrix::inverse(double epsilon) const {
    if (rows != cols) throw std::logic_error("Inverse is defined for square matrices");
    size_t n = rows;
    Matrix augmented(n, 2 * n);

    // Заполнение расширенной матрицы
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            augmented[i][j] = data[i][j];
            augmented[i][j + n] = (i == j) ? 1.0 : 0.0;
        }
    }

    // Прямой ход метода Гаусса-Жордана
    for (size_t i = 0; i < n; ++i) {
        // Поиск опорного элемента
        size_t pivot = i;
        for (size_t j = i; j < n; ++j) {
            if (std::abs(augmented[j][i]) > std::abs(augmented[pivot][i])) {
                pivot = j;
            }
        }

        if (std::abs(augmented[pivot][i]) < epsilon) {
            throw std::logic_error("Matrix is singular");
        }

        // Обмен строк
        if (pivot != i) {
            double* tmp = augmented.data[i];
            augmented.data[i] = augmented.data[pivot];
            augmented.data[pivot] = tmp;
        }

        // Нормализация строки
        double div = augmented[i][i];
        for (size_t j = 0; j < 2 * n; ++j) {
            augmented[i][j] /= div;
            // Округление до нуля
            if (std::abs(augmented[i][j]) < epsilon) augmented[i][j] = 0.0;
        }

        // Обнуление столбца
        for (size_t j = 0; j < n; ++j) {
            if (j != i && std::abs(augmented[j][i]) > epsilon) {
                double factor = augmented[j][i];
                for (size_t k = 0; k < 2 * n; ++k) {
                    augmented[j][k] -= factor * augmented[i][k];
                    // Округление до нуля
                    if (std::abs(augmented[j][k]) < epsilon) augmented[j][k] = 0.0;
                }
            }
        }
    }

    // Извлекаем обрат матрицу
    Matrix inverse(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            inverse[i][j] = augmented[i][j + n];
            // присваиваем 0 если значение меньше epsilon
            if (std::abs(inverse[i][j]) < epsilon) {
                inverse[i][j] = 0.0;
            }
        }
    }
    return inverse;
}
