#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

/* matrix structure */

typedef struct {
    int rows;
    int cols;
    double *data;
} matrix;

/* index macro (Numerical Recipes style) */

#define MAT(m,i,j) (m->data[(i)*(m->cols) + (j)])

/* memory */

matrix* matrix_create(int rows, int cols);
void matrix_free(matrix *m);

#endif