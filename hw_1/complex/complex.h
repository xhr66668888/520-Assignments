#ifndef COMPLEX_H
#define COMPLEX_H

struct complex {
    double real;
    double im;
};

struct polar {
    double r;
    double theta;
};

struct complex add(struct complex a, struct complex b);
struct complex subtract(struct complex a, struct complex b);
struct complex negate(struct complex a);
struct complex multiply(struct complex a, struct complex b);
struct complex divide(struct complex a, struct complex b);
struct complex conjugate(struct complex a);
struct polar to_polar(struct complex a);
struct complex power(struct complex a, int exponent);
double magnitude(struct complex a);

int equals(struct complex a, struct complex b);
int is_real(struct complex a);
int is_imaginary(struct complex a);
int is_zero(struct complex a);

#endif
