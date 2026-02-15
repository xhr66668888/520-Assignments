#include <math.h>
#include <float.h>
#include <assert.h>
#include "typed_array.h"
#include "matrix.h"
#include "gtest/gtest.h"

namespace {

    /* ======= TypedArray tests ======= */

    TEST(TypedArray, PushAndPop) {
        TypedArray<int> a;
        a.push(1);
        a.push(2);
        a.push(3);
        EXPECT_EQ(a.size(), 3);
        EXPECT_EQ(a.get(0), 1);

        // pop should give us the last thing we pushed
        EXPECT_EQ(a.pop(), 3);
        EXPECT_EQ(a.pop(), 2);
        EXPECT_EQ(a.size(), 1);
    }

    TEST(TypedArray, PushFrontAndPopFront) {
        TypedArray<int> a;
        a.push_front(3);
        a.push_front(2);
        a.push_front(1);
        // should be [1, 2, 3] now
        EXPECT_EQ(a.get(0), 1);
        EXPECT_EQ(a.get(1), 2);
        EXPECT_EQ(a.get(2), 3);

        EXPECT_EQ(a.pop_front(), 1);
        EXPECT_EQ(a.pop_front(), 2);
        EXPECT_EQ(a.size(), 1);
    }

    TEST(TypedArray, PopEmptyThrows) {
        TypedArray<int> a;
        EXPECT_THROW(a.pop(), std::range_error);
        EXPECT_THROW(a.pop_front(), std::range_error);
    }

    TEST(TypedArray, Concat) {
        TypedArray<int> a;
        a.set(0, 0);
        a.set(1, 1);
        TypedArray<int> b = a.concat(a).concat(a); // yields a new array [0,1,0,1,0,1]
                                                    // Should leave a untouched.
        EXPECT_EQ(b.size(), 6);
        EXPECT_EQ(b.get(0), 0);
        EXPECT_EQ(b.get(1), 1);
        EXPECT_EQ(b.get(2), 0);
        EXPECT_EQ(b.get(3), 1);
        EXPECT_EQ(b.get(4), 0);
        EXPECT_EQ(b.get(5), 1);
        // a should still be [0,1]
        EXPECT_EQ(a.size(), 2);
    }

    TEST(TypedArray, Reverse) {
        TypedArray<int> a;
        a.push(1);
        a.push(2);
        a.push(3);
        a.push(4);
        a.reverse();
        EXPECT_EQ(a.get(0), 4);
        EXPECT_EQ(a.get(1), 3);
        EXPECT_EQ(a.get(2), 2);
        EXPECT_EQ(a.get(3), 1);

        // double reverse should give original back
        a.reverse().reverse();
        EXPECT_EQ(a.get(0), 4);
    }

    TEST(TypedArray, PlusOperator) {
        TypedArray<int> a;
        a.set(0, 0);
        a.set(1, 1);
        TypedArray<int> b = a + a + a; // yields [0,1,0,1,0,1]
        EXPECT_EQ(b.size(), 6);
        EXPECT_EQ(b.get(0), 0);
        EXPECT_EQ(b.get(5), 1);
    }

    TEST(TypedArray, MixedPushPop) {
        TypedArray<double> a;
        a.push(1.5);
        a.push_front(0.5);
        a.push(2.5);
        // should be [0.5, 1.5, 2.5]
        EXPECT_DOUBLE_EQ(a.pop_front(), 0.5);
        EXPECT_DOUBLE_EQ(a.pop(), 2.5);
        EXPECT_EQ(a.size(), 1);
    }

    /* ======= Matrix tests ======= */

    TEST(Matrix, Constructors) {
        // default
        Matrix m1;
        EXPECT_TRUE(m1.isEmpty());
        EXPECT_EQ(m1.rows(), 0);

        // size constructor, should be zeros
        Matrix m2(3, 4);
        EXPECT_EQ(m2.rows(), 3);
        EXPECT_EQ(m2.cols(), 4);
        EXPECT_DOUBLE_EQ(m2(0, 0), 0.0);
        EXPECT_DOUBLE_EQ(m2(2, 3), 0.0);

        // value constructor
        Matrix m3(2, 3, 5.0);
        EXPECT_DOUBLE_EQ(m3(0, 0), 5.0);
        EXPECT_DOUBLE_EQ(m3(1, 2), 5.0);
    }

    TEST(Matrix, InitializerList) {
        Matrix A = {{1, 2}, {3, 4}};
        EXPECT_EQ(A.rows(), 2);
        EXPECT_EQ(A.cols(), 2);
        EXPECT_DOUBLE_EQ(A(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(A(0, 1), 2.0);
        EXPECT_DOUBLE_EQ(A(1, 0), 3.0);
        EXPECT_DOUBLE_EQ(A(1, 1), 4.0);

        // unequal row size, short row padded with 0
        Matrix m = {{1, 2, 3}, {4, 5}};
        EXPECT_EQ(m.cols(), 3);
        EXPECT_DOUBLE_EQ(m(1, 2), 0.0);
    }

    TEST(Matrix, CopySemantics) {
        Matrix A = {{1, 2}, {3, 4}};

        // copy constructor - modifying copy shouldnt touch original
        Matrix B(A);
        B(0, 0) = 99;
        EXPECT_DOUBLE_EQ(A(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(B(0, 0), 99.0);

        // copy assignment
        Matrix C;
        C = A;
        C(1, 1) = -1;
        EXPECT_DOUBLE_EQ(A(1, 1), 4.0);
    }

    TEST(Matrix, BoundsChecking) {
        Matrix m(2, 2);
        EXPECT_THROW(m.at(5, 0), std::out_of_range);
        EXPECT_THROW(m.at(0, 5), std::out_of_range);

        // valid access should work fine
        Matrix m2 = {{10, 20}, {30, 40}};
        EXPECT_DOUBLE_EQ(m2.at(0, 0), 10.0);
        EXPECT_DOUBLE_EQ(m2.at(1, 1), 40.0);
    }

    TEST(Matrix, Addition) {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = {{5, 6}, {7, 8}};
        Matrix C = A + B;
        EXPECT_DOUBLE_EQ(C(0, 0), 6.0);
        EXPECT_DOUBLE_EQ(C(0, 1), 8.0);
        EXPECT_DOUBLE_EQ(C(1, 0), 10.0);
        EXPECT_DOUBLE_EQ(C(1, 1), 12.0);

        // diff sizes should throw
        Matrix D(3, 3);
        EXPECT_THROW(A + D, std::invalid_argument);
    }

    TEST(Matrix, SubtractionAndNegation) {
        Matrix A = {{5, 6}, {7, 8}};
        Matrix B = {{1, 2}, {3, 4}};
        Matrix C = A - B;
        EXPECT_DOUBLE_EQ(C(0, 0), 4.0);
        EXPECT_DOUBLE_EQ(C(1, 1), 4.0);

        // unary negation
        Matrix D = -B;
        EXPECT_DOUBLE_EQ(D(0, 0), -1.0);
        EXPECT_DOUBLE_EQ(D(0, 1), -2.0);
    }

    TEST(Matrix, MatrixMultiplication) {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = {{5, 6}, {7, 8}};
        Matrix D = A * B;
        // manually computed: [1*5+2*7, 1*6+2*8] = [19, 22]
        //                    [3*5+4*7, 3*6+4*8] = [43, 50]
        EXPECT_DOUBLE_EQ(D(0, 0), 19.0);
        EXPECT_DOUBLE_EQ(D(0, 1), 22.0);
        EXPECT_DOUBLE_EQ(D(1, 0), 43.0);
        EXPECT_DOUBLE_EQ(D(1, 1), 50.0);

        // wrong dimensions
        Matrix E(2, 3);
        Matrix F(2, 3);
        EXPECT_THROW(E * F, std::invalid_argument);
    }

    TEST(Matrix, ScalarOperations) {
        Matrix A = {{1, 2}, {3, 4}};

        // scalar * matrix and matrix * scalar
        Matrix E = 2.0 * A;
        EXPECT_DOUBLE_EQ(E(0, 0), 2.0);
        EXPECT_DOUBLE_EQ(E(1, 1), 8.0);

        Matrix F = A * 3.0;
        EXPECT_DOUBLE_EQ(F(0, 0), 3.0);

        // division
        Matrix G = {{10, 20}, {30, 40}};
        Matrix H = G / 10.0;
        EXPECT_DOUBLE_EQ(H(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(H(1, 1), 4.0);
    }

    TEST(Matrix, CompoundAssignment) {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = {{1, 1}, {1, 1}};

        A += B;
        EXPECT_DOUBLE_EQ(A(0, 0), 2.0);
        EXPECT_DOUBLE_EQ(A(1, 1), 5.0);

        A *= 2.0;
        EXPECT_DOUBLE_EQ(A(0, 0), 4.0);
    }

    TEST(Matrix, Comparision) {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = {{1, 2}, {3, 4}};
        EXPECT_TRUE(A == B);
        EXPECT_FALSE(A != B);

        // different sizes should not be equal
        Matrix C(3, 3);
        EXPECT_TRUE(A != C);

        // epsilon comparision - very close values should be equal
        Matrix D = {{1.0000001, 2.0}, {3.0, 4.0}};
        EXPECT_TRUE(A == D);
    }

    TEST(Matrix, Transpose) {
        Matrix A = {{1, 2, 3}, {4, 5, 6}};
        Matrix F = A.transpose();
        EXPECT_EQ(F.rows(), 3);
        EXPECT_EQ(F.cols(), 2);
        EXPECT_DOUBLE_EQ(F(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(F(0, 1), 4.0);
        EXPECT_DOUBLE_EQ(F(2, 0), 3.0);

        // transposing twice should give back original
        EXPECT_TRUE(A == A.transpose().transpose());
    }

    TEST(Matrix, TraceAndDiagonal) {
        Matrix A = {{1, 2}, {3, 4}};
        EXPECT_NEAR(A.trace(), 5.0, 0.001);

        // non square should throw
        Matrix B(2, 3);
        EXPECT_THROW(B.trace(), std::logic_error);

        // diagonal extraction
        Matrix d = A.diagonal();
        EXPECT_EQ(d.rows(), 2);
        EXPECT_EQ(d.cols(), 1);
        EXPECT_DOUBLE_EQ(d(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(d(1, 0), 4.0);
    }

    TEST(Matrix, FillAndNorm) {
        Matrix A(2, 3);
        A.fill(7.0);
        EXPECT_DOUBLE_EQ(A(0, 0), 7.0);
        EXPECT_DOUBLE_EQ(A(1, 2), 7.0);

        // norm = sqrt(sum of squares)
        Matrix B = {{1, 2}, {3, 4}};
        double n = B.norm();
        EXPECT_NEAR(n, sqrt(30.0), 0.001);
    }

    TEST(Matrix, StaticFactories) {
        // identity
        Matrix I = Matrix::identity(3);
        EXPECT_DOUBLE_EQ(I(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(I(1, 1), 1.0);
        EXPECT_DOUBLE_EQ(I(0, 1), 0.0);
        // trace of identity = n
        EXPECT_NEAR(I.trace(), 3.0, 0.001);

        // zeros
        Matrix Z = Matrix::zeros(2, 3);
        EXPECT_DOUBLE_EQ(Z(0, 0), 0.0);

        // ones
        Matrix O = Matrix::ones(2, 2);
        EXPECT_DOUBLE_EQ(O(0, 0), 1.0);

        // diagonal from vector
        std::vector<double> v = {1, 2, 3};
        Matrix D = Matrix::diagonal(v);
        EXPECT_EQ(D.rows(), 3);
        EXPECT_DOUBLE_EQ(D(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(D(1, 1), 2.0);
        EXPECT_DOUBLE_EQ(D(0, 1), 0.0);
    }

    TEST(Matrix, SquareAndEmpty) {
        Matrix A(3, 3);
        EXPECT_TRUE(A.isSquare());

        Matrix B(2, 3);
        EXPECT_FALSE(B.isSquare());

        Matrix C;
        EXPECT_TRUE(C.isEmpty());
    }

    TEST(Matrix, IdentityMultiply) {
        // sanity check: A * I should give A
        Matrix A = {{1, 2}, {3, 4}};
        Matrix I = Matrix::identity(2);
        Matrix res = A * I;
        EXPECT_TRUE(res == A);
    }

} // namespace
