#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "complex.h"
#include "unit_test.h"

void test_pass(const char* test_name) {
    printf("[PASS] %s\n", test_name);
}

void run_tests() {
    struct complex c1 = {1.0, 2.0};
    struct complex c2 = {3.0, 4.0};
    struct complex zero = {0.0, 0.0};
    struct complex res;

    res = add(c1, c2);
    assert(res.real == 4.0 && res.im == 6.0);
    test_pass("Add");

    res = subtract(c1, c2);
    assert(res.real == -2.0 && res.im == -2.0);
    test_pass("Subtract");

    res = multiply(c1, c2);
    assert(res.real == -5.0 && res.im == 10.0);
    test_pass("Multiply");

    res = divide(c1, c2);
    assert(fabs(res.real - 0.44) < 0.0001);
    assert(fabs(res.im - 0.08) < 0.0001);
    test_pass("Divide");

    res = divide(c1, zero);
    assert(isinf(res.real) && isinf(res.im));
    test_pass("Divide by Zero");

    res = conjugate(c1);
    assert(res.real == 1.0 && res.im == -2.0);
    test_pass("Conjugate");

    double mag = magnitude(c2);
    assert(mag == 5.0);
    test_pass("Magnitude");

    struct complex c3 = {1.0, 1.0};
    res = power(c3, 2);
    assert(fabs(res.real) < 0.0001 && fabs(res.im - 2.0) < 0.0001);
    test_pass("Power (Square)");

    struct complex c4 = {0.0, 2.0};
    res = power(c4, -1);
    assert(fabs(res.real) < 0.0001); 
    assert(fabs(res.im - (-0.5)) < 0.0001);
    test_pass("Power (Negative Exponent)");

    struct complex real_num = {5.0, 0.0};
    struct complex imag_num = {0.0, 5.0};
    
    assert(is_real(real_num) == 1);
    assert(is_real(imag_num) == 0);
    test_pass("Is Real");

    assert(is_imaginary(imag_num) == 1);
    assert(is_imaginary(real_num) == 0);
    assert(is_imaginary(zero) == 0);
    test_pass("Is Imaginary");
}
