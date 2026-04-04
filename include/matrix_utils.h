
#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include "matrix.h"

void matrix_fill_zero(matrix *m);
void matrix_fill_identity(matrix *m);
matrix* matrix_copy(const matrix *m);
matrix* matrix_transpose(const matrix *m);
void matrix_print(const matrix *m);

#endif /* MATRIX_UTILS_H */