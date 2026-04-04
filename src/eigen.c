#include <stdlib.h>
#include <math.h>
#include "eigen.h"

static double vector_norm(const double *v, int n)
{
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += v[i] * v[i];
    }
    return sqrt(sum);
}

static void normalize(double *v, int n)
{
    double norm = vector_norm(v, n);
    if (norm == 0.0) {
        return;
    }

    for (int i = 0; i < n; i++) {
        v[i] /= norm;
    }
}

static void mat_vec_mul(const matrix *a, const double *x, double *y)
{
    int n = a->rows;

    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++) {
            sum += a->data[i * n + j] * x[j];
        }
        y[i] = sum;
    }
}

static double dot_product(const double *a, const double *b, int n)
{
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

int power_method(const matrix *a, double *eigenvalue, double *eigenvector, int max_iter, double tol)
{
    if (a == NULL || eigenvalue == NULL || eigenvector == NULL) {
        return -1;
    }

    if (a->rows != a->cols) {
        return -1;
    }

    int n = a->rows;

    double *x = (double *)malloc(sizeof(double) * n);
    double *y = (double *)malloc(sizeof(double) * n);

    if (x == NULL || y == NULL) {
        free(x);
        free(y);
        return -1;
    }

    for (int i = 0; i < n; i++) {
        x[i] = 1.0;
    }

    normalize(x, n);

    double lambda_old = 0.0;
    double lambda_new = 0.0;

    for (int iter = 0; iter < max_iter; iter++) {

        mat_vec_mul(a, x, y);

        lambda_new = dot_product(x, y, n);

        normalize(y, n);

        double diff = fabs(lambda_new - lambda_old);

        for (int i = 0; i < n; i++) {
            x[i] = y[i];
        }

        if (diff < tol) {
            break;
        }

        lambda_old = lambda_new;
    }

    *eigenvalue = lambda_new;

    for (int i = 0; i < n; i++) {
        eigenvector[i] = x[i];
    }

    free(x);
    free(y);

    return 0;
}
