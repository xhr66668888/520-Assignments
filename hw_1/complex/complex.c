#include "complex.h"
#include <math.h>

struct complex add(struct complex a, struct complex b) {
    struct complex res;
    res.real = a.real + b.real;
    res.im = a.im + b.im;
    return res;
}

struct complex subtract(struct complex a, struct complex b) {
    struct complex res;
    res.real = a.real - b.real;
    res.im = a.im - b.im;
    return res;
}

struct complex negate(struct complex a) {
    struct complex res;
    res.real = -a.real;
    res.im = -a.im;
    return res;
}

struct complex multiply(struct complex a, struct complex b) {
    struct complex res;
    res.real = a.real * b.real - a.im * b.im;
    res.im = a.real * b.im + a.im * b.real;
    return res;
}

struct complex divide(struct complex a, struct complex b) {
    struct complex res;
    double denominator = b.real * b.real + b.im * b.im;
    if (denominator == 0) {
        res.real = INFINITY;
        res.im = INFINITY;
        return res;
    }
    res.real = (a.real * b.real + a.im * b.im) / denominator;
    res.im = (a.im * b.real - a.real * b.im) / denominator;
    return res;
}

struct complex conjugate(struct complex a) {
    struct complex res;
    res.real = a.real;
    res.im = -a.im;
    return res;
}

struct polar to_polar(struct complex a) {
    struct polar res;
    res.r = sqrt(a.real * a.real + a.im * a.im);
    res.theta = atan2(a.im, a.real);
    return res;
}

struct complex power(struct complex a, int exponent) {
    struct complex res;
    struct polar p = to_polar(a);
    double new_r = pow(p.r, exponent);
    double new_theta = p.theta * exponent;
    res.real = new_r * cos(new_theta);
    res.im = new_r * sin(new_theta);
    return res;
}

double magnitude(struct complex a) {
    return sqrt(a.real * a.real + a.im * a.im);
}

int equals(struct complex a, struct complex b) {
    return (a.real == b.real && a.im == b.im);
}

int is_real(struct complex a) {
    return (a.im == 0);
}

int is_imaginary(struct complex a) {
    return (a.real == 0 && a.im != 0);
}

int is_zero(struct complex a) {
    return (a.real == 0 && a.im == 0);
}
