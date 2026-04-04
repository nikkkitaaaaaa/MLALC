#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "matrix_ops.h"

double get_time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    return ts.tv_sec*1000.0 + ts.tv_nsec/1e6;
}

void fill_random(matrix *m)
{
    int total = m->rows * m->cols;

    for(int i=0;i<total;i++)
        m->data[i] = (double)rand()/RAND_MAX;
}

int main()
{
    int n = 1000;

    printf("Testing with %dx%d matrix\n",n,n);

    matrix *A = matrix_create(n,n);
    matrix *B = matrix_create(n,n);

    fill_random(A);
    fill_random(B);

    double start,end;

    /* ADD */

    start = get_time_ms();
    matrix *C = matrix_add(A,B);
    end = get_time_ms();

    printf("C add time: %.3f ms\n",end-start);
    matrix_free(C);

    /* SUBTRACT */

    start = get_time_ms();
    C = matrix_subtract(A,B);
    end = get_time_ms();

    printf("C subtract time: %.3f ms\n",end-start);
    matrix_free(C);

    /* SCALAR MULTIPLY */

    start = get_time_ms();
    C = matrix_scalar_multiply(A,2.5);
    end = get_time_ms();

    printf("C scalar multiply time: %.3f ms\n",end-start);
    matrix_free(C);

    /* MATRIX MULTIPLY */

    start = get_time_ms();
    C = matrix_multiply(A,B);
    end = get_time_ms();

    printf("C matrix multiply time: %.3f ms\n",end-start);
    matrix_free(C);

    matrix_free(A);
    matrix_free(B);

    return 0;
}