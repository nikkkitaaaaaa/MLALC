#include <stdlib.h>
#include <string.h>
#include "matrix.h"

matrix* matrix_create(int rows, int cols)
{
    matrix *m;

    if(rows <= 0 || cols <= 0)
        return NULL;

    m = (matrix*) malloc(sizeof(matrix));

    if(!m)
        return NULL;

    m->rows = rows;
    m->cols = cols;

    m->data = (double*) calloc(rows*cols, sizeof(double));

    if(!m->data){
        free(m);
        return NULL;
    }

    return m;
}

void matrix_free(matrix *m)
{
    if(!m)
        return;

    free(m->data);
    free(m);
}