#include "matrix.h"
#include <algorithm>

Matrix::Matrix() : num_rows(0), num_cols(0) {}

Matrix::Matrix(size_t rows, size_t cols) 
    : num_rows(rows), num_cols(cols), data(rows * cols, 0.0) {}

Matrix::Matrix(size_t rows, size_t cols, double value)
    : num_rows(rows), num_cols(cols), data(rows * cols, value) {}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list) {
    num_rows = list.size();
    num_cols = 0;

    // need to find the longest row first
    for (auto& row : list) {
        if (row.size() > num_cols) {
            num_cols = row.size();
        }
    }

    data.resize(num_rows * num_cols, 0.0);

    size_t r = 0;
    for (auto& row : list) {
        size_t c = 0;
        for (auto& val : row) {
            data[r * num_cols + c] = val;
            c++;
        }
        r++;
    }
}

Matrix::Matrix(const Matrix& other) 
    : num_rows(other.num_rows), num_cols(other.num_cols), data(other.data) {}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        num_rows = other.num_rows;
        num_cols = other.num_cols;
        data = other.data;
    }
    return *this;
}

double& Matrix::operator()(size_t row, size_t col) {
    return data[row * num_cols + col];
}

const double& Matrix::operator()(size_t row, size_t col) const {
    return data[row * num_cols + col];
}

double& Matrix::at(size_t row, size_t col) {
    if (row >= num_rows || col >= num_cols) {
        throw std::out_of_range("index out of range");
    }
    return data[row * num_cols + col];
}

const double& Matrix::at(size_t row, size_t col) const {
    if (row >= num_rows || col >= num_cols) {
        throw std::out_of_range("index out of range");
    }
    return data[row * num_cols + col];
}

size_t Matrix::rows() const { return num_rows; }
size_t Matrix::cols() const { return num_cols; }
bool Matrix::isEmpty() const { return num_rows == 0 || num_cols == 0; }
bool Matrix::isSquare() const { return num_rows == num_cols; }

Matrix Matrix::operator+(const Matrix& other) const {
    if (num_rows != other.num_rows || num_cols != other.num_cols) {
        throw std::invalid_argument("cant add matrices with different dimensions");
    }
    Matrix res(num_rows, num_cols);
    for (size_t i = 0; i < data.size(); i++) {
        res.data[i] = data[i] + other.data[i];
    }
    return res;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (num_rows != other.num_rows || num_cols != other.num_cols) {
        throw std::invalid_argument("cant subtract matrices with different dimensions");
    }
    Matrix res(num_rows, num_cols);
    for (size_t i = 0; i < data.size(); i++) {
        res.data[i] = data[i] - other.data[i];
    }
    return res;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (num_cols != other.num_rows) {
        throw std::invalid_argument("matrix dimensions dont work for multiply");
    }
    Matrix res(num_rows, other.num_cols);
    for (size_t i = 0; i < num_rows; i++) {
        for (size_t j = 0; j < other.num_cols; j++) {
            double sum = 0;
            for (size_t k = 0; k < num_cols; k++) {
                sum = sum + data[i * num_cols + k] * other.data[k * other.num_cols + j];
            }
            res.data[i * other.num_cols + j] = sum;
        }
    }
    return res;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix res(num_rows, num_cols);
    for (size_t i = 0; i < data.size(); i++) {
        res.data[i] = data[i] * scalar;
    }
    return res;
}

// need this so we can write 2.0 * A
Matrix operator*(double scalar, const Matrix& m) {
    return m * scalar;
}

Matrix Matrix::operator/(double scalar) const {
    // just let it happen, if scalar is 0 youll get inf which is fine i think
    Matrix res(num_rows, num_cols);
    for (size_t i = 0; i < data.size(); i++) {
        res.data[i] = data[i] / scalar;
    }
    return res;
}

Matrix Matrix::operator-() const {
    Matrix res(num_rows, num_cols);
    for (size_t i = 0; i < data.size(); i++) {
        res.data[i] = -data[i];
    }
    return res;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    *this = *this + other;
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    *this = *this - other;
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    *this = *this * other;
    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = data[i] * scalar;
    }
    return *this;
}

Matrix& Matrix::operator/=(double scalar) {
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = data[i] / scalar;
    }
    return *this;
}

// use epsilon so floating point doesnt mess things up
bool Matrix::operator==(const Matrix& other) const {
    if (num_rows != other.num_rows || num_cols != other.num_cols) {
        return false;
    }
    for (size_t i = 0; i < data.size(); i++) {
        if (std::fabs(data[i] - other.data[i]) > EPSILON) {
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

Matrix Matrix::transpose() const {
    Matrix res(num_cols, num_rows);
    for (size_t r = 0; r < num_rows; r++) {
        for (size_t c = 0; c < num_cols; c++) {
            res.data[c * num_rows + r] = data[r * num_cols + c];
        }
    }
    return res;
}

double Matrix::trace() const {
    if (!isSquare()) {
        throw std::logic_error("trace needs a square matrix");
    }
    double sum = 0;
    for (size_t i = 0; i < num_rows; i++) {
        sum = sum + data[i * num_cols + i];
    }
    return sum;
}

// get the diagonal elements as a column vector
Matrix Matrix::diagonal() const {
    size_t n = std::min(num_rows, num_cols);
    Matrix res(n, 1);
    for (size_t i = 0; i < n; i++) {
        res.data[i] = data[i * num_cols + i];
    }
    return res;
}

void Matrix::fill(double value) {
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = value;
    }
}

double Matrix::norm() const {
    double sum = 0;
    for (size_t i = 0; i < data.size(); i++) {
        sum = sum + data[i] * data[i];
    }
    return std::sqrt(sum);
}

Matrix Matrix::identity(size_t n) {
    Matrix res(n, n);
    for (size_t i = 0; i < n; i++) {
        res.data[i * n + i] = 1.0;
    }
    return res;
}

Matrix Matrix::zeros(size_t rows, size_t cols) {
    return Matrix(rows, cols);
}

Matrix Matrix::ones(size_t rows, size_t cols) {
    return Matrix(rows, cols, 1.0);
}

Matrix Matrix::diagonal(const std::vector<double>& diag) {
    size_t n = diag.size();
    Matrix res(n, n);
    for (size_t i = 0; i < n; i++) {
        res.data[i * n + i] = diag[i];
    }
    return res;
}
