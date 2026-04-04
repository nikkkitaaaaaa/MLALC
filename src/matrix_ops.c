#include <stdlib.h>
#include "matrix_ops.h"


/* matrix addition */

matrix* matrix_add(const matrix *restrict a,
                   const matrix *restrict b)
{
    int total,i;
    matrix *r;

    if(!a || !b)
        return NULL;

    if(a->rows != b->rows || a->cols != b->cols)
        return NULL;

    r = matrix_create(a->rows,a->cols);
    if(!r)
        return NULL;

    total = a->rows * a->cols;

    const double *ad = a->data;
    const double *bd = b->data;
    double *rd = r->data;

    for(i=0;i<total;i++)
        rd[i] = ad[i] + bd[i];

    return r;
}


/* matrix subtraction */

matrix* matrix_subtract(const matrix *restrict a,
                        const matrix *restrict b)
{
    int total,i;
    matrix *r;

    if(!a || !b)
        return NULL;

    if(a->rows != b->rows || a->cols != b->cols)
        return NULL;

    r = matrix_create(a->rows,a->cols);
    if(!r)
        return NULL;

    total = a->rows * a->cols;

    const double *ad = a->data;
    const double *bd = b->data;
    double *rd = r->data;

    for(i=0;i<total;i++)
        rd[i] = ad[i] - bd[i];

    return r;
}


/* scalar multiplication */

matrix* matrix_scalar_multiply(const matrix *a,double s)
{
    int total,i;
    matrix *r;

    if(!a)
        return NULL;

    r = matrix_create(a->rows,a->cols);
    if(!r)
        return NULL;

    total = a->rows * a->cols;

    const double *ad = a->data;
    double *rd = r->data;

    for(i=0;i<total;i++)
        rd[i] = ad[i] * s;

    return r;
}


/* matrix multiplication */

matrix* matrix_multiply(const matrix *restrict a,
                        const matrix *restrict b)
{
    int i,j,k;
    int m,n,p;

    matrix *c;

    if(!a || !b)
        return NULL;

    if(a->cols != b->rows)
        return NULL;

    m = a->rows;
    n = a->cols;
    p = b->cols;

    c = matrix_create(m,p);
    if(!c)
        return NULL;

    double *cd = c->data;
    const double *ad = a->data;
    const double *bd = b->data;

    for(i=0;i<m;i++)
    {
        double *rowC = cd + i*p;
        const double *rowA = ad + i*n;

        for(k=0;k<n;k++)
        {
            double aik = rowA[k];
            const double *rowB = bd + k*p;

            for(j=0;j<p;j++)
                rowC[j] += aik * rowB[j];
        }
    }

    return c;
}