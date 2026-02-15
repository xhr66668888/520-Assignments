#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <cmath>

class Matrix {

public:

    // constructors
    Matrix();
    Matrix(size_t rows, size_t cols);
    Matrix(size_t rows, size_t cols, double value);
    Matrix(std::initializer_list<std::initializer_list<double>> list);
    Matrix(const Matrix& other);

    Matrix& operator=(const Matrix& other);

    // element access stuff
    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;
    double& at(size_t row, size_t col);  // throws if out of bounds
    const double& at(size_t row, size_t col) const;

    size_t rows() const;
    size_t cols() const;
    bool isEmpty() const;
    bool isSquare() const;

    // operators
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(double scalar) const;
    friend Matrix operator*(double scalar, const Matrix& m);
    Matrix operator/(double scalar) const;
    Matrix operator-() const;

    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);
    Matrix& operator*=(double scalar);
    Matrix& operator/=(double scalar);

    // comparision
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    // other operations
    Matrix transpose() const;
    double trace() const;
    Matrix diagonal() const;
    void fill(double value);
    double norm() const;   // frobenius

    // static factory stuff
    static Matrix identity(size_t n);
    static Matrix zeros(size_t rows, size_t cols);
    static Matrix ones(size_t rows, size_t cols);
    static Matrix diagonal(const std::vector<double>& diag);

private:
    std::vector<double> data;
    size_t num_rows;
    size_t num_cols;
    static constexpr double EPSILON = 1e-6;
};

#endif
