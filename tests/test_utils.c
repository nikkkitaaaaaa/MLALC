#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "matrix_utils.h"

double get_time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    return ts.tv_sec*1000.0 + ts.tv_nsec/1e6;
}

int main()
{
    int n = 2000;
    printf("Testing with 2000×2000 matrix\n");
    matrix *A = matrix_create(n,n);

    for(int i=0;i<n*n;i++)
        A->data[i] = (double)rand()/RAND_MAX;

    double start,end;

    /* COPY TEST */

    start = get_time_ms();

    matrix *B = matrix_copy(A);

    end = get_time_ms();

    printf("C copy time: %.3f ms\n",end-start);

    matrix_free(B);

    /* TRANSPOSE TEST */

    start = get_time_ms();

    matrix *T = matrix_transpose(A);

    end = get_time_ms();

    printf("C transpose time: %.3f ms\n",end-start);

    matrix_free(T);
    matrix_free(A);

    return 0;
}