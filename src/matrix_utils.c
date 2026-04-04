#include <stdio.h>
#include <string.h>
#include "matrix_utils.h"


/* set all elements to zero */

void matrix_fill_zero(matrix *m)
{
    int total;

    if(!m || !m->data)
        return;

    total = m->rows * m->cols;

    memset(m->data, 0, total * sizeof(double));
}


/* create identity matrix */

void matrix_fill_identity(matrix *m)
{
    int i,n;

    if(!m || !m->data)
        return;

    if(m->rows != m->cols)
        return;

    matrix_fill_zero(m);

    n = m->rows;

    for(i=0;i<n;i++)
        MAT(m,i,i) = 1.0;
}


/* copy matrix */

matrix* matrix_copy(const matrix *m)
{
    matrix *c;
    int total;

    if(!m || !m->data)
        return NULL;

    c = matrix_create(m->rows, m->cols);

    if(!c)
        return NULL;

    total = m->rows * m->cols;

    memcpy(c->data, m->data, total * sizeof(double));

    return c;
}


/* transpose matrix */

matrix* matrix_transpose(const matrix *m)
{
    matrix *t;
    int i,j;

    if(!m || !m->data)
        return NULL;

    t = matrix_create(m->cols, m->rows);

    if(!t)
        return NULL;

    for(i=0;i<m->rows;i++)
        for(j=0;j<m->cols;j++)
            MAT(t,j,i) = MAT(m,i,j);

    return t;
}


/* print matrix (for debugging) */

void matrix_print(const matrix *m)
{
    int i,j;

    if(!m || !m->data){
        printf("(null matrix)\n");
        return;
    }

    for(i=0;i<m->rows;i++)
    {
        for(j=0;j<m->cols;j++)
            printf("%10.4f ", MAT(m,i,j));

        printf("\n");
    }
}