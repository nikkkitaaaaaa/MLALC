#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include "matrix.h"

/* elementwise operations */

matrix* matrix_add(const matrix *restrict a,
                   const matrix *restrict b);

matrix* matrix_subtract(const matrix *restrict a,
                        const matrix *restrict b);

matrix* matrix_scalar_multiply(const matrix *a,
                               double s);

/* matrix multiplication */

matrix* matrix_multiply(const matrix *restrict a,
                        const matrix *restrict b);

#endif