#ifndef CORDICCART2POL_H
#define CORDICCART2POL_H

#include <ap_fixed.h>

#define NO_ITER 16
#define ITER_RUN 10

#define W			14 // Total size of fixed-point representation. Must be W >= (I+MAN_BITS)
#define I			3  // Size of integer part of fixed-point (1 bit sign + 1 bit integer)
#define MAN_BITS	11  // Size of fractional part. Vary this number to increase/decrease the resolution.
#define ET 0.0005


typedef int   coef_t;
typedef float data_t;
typedef float acc_t;
typedef ap_fixed<W, I, AP_RND, AP_WRAP, 1> data_fixed;

void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta);

#endif
