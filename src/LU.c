#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "LU.h"
#include "matrix_utils.h"

/* Optimized row swap using memcpy */
static void swap_rows(matrix *restrict m, int r1, int r2)
{
    if (m == NULL || r1 == r2) return;

    double *row1 = m->data + r1 * m->cols;
    double *row2 = m->data + r2 * m->cols;
    double *temp = malloc(m->cols * sizeof(double));
    if (!temp) return;  /* fallback – should not happen for reasonable sizes */

    memcpy(temp, row1, m->cols * sizeof(double));
    memcpy(row1, row2, m->cols * sizeof(double));
    memcpy(row2, temp, m->cols * sizeof(double));

    free(temp);
}

int lu_decompose(const matrix *restrict a,
                 matrix *restrict l,
                 matrix *restrict u,
                 int *restrict p)
{
    if (!a || !l || !u || !p) return -1;
    if (a->rows != a->cols) return -1;

    int n = a->rows;
    if (l->rows != n || l->cols != n || u->rows != n || u->cols != n)
        return -1;

    /* Zero L and copy A into U (use memcpy for speed) */
    matrix_fill_zero(l);
    memcpy(u->data, a->data, n * n * sizeof(double));

    /* Set L diagonal to 1.0 */
    for (int i = 0; i < n; i++)
        l->data[i * n + i] = 1.0;

    /* Permutation vector */
    for (int i = 0; i < n; i++)
        p[i] = i;

    double *u_data = u->data;
    double *l_data = l->data;

    for (int k = 0; k < n; k++) {
        /* Find pivot row (partial pivoting) */
        double max_val = fabs(u_data[k * n + k]);
        int pivot_row = k;
        for (int i = k + 1; i < n; i++) {
            double val = fabs(u_data[i * n + k]);
            if (val > max_val) {
                max_val = val;
                pivot_row = i;
            }
        }
        if (max_val == 0.0) return -1;   /* singular matrix */

        if (pivot_row != k) {
            /* Swap rows in U */
            swap_rows(u, k, pivot_row);

            /* Swap rows in L (only left of diagonal) */
            for (int j = 0; j < k; j++) {
                double tmp = l_data[k * n + j];
                l_data[k * n + j] = l_data[pivot_row * n + j];
                l_data[pivot_row * n + j] = tmp;
            }

            /* Swap permutation vector */
            int tmp_p = p[k];
            p[k] = p[pivot_row];
            p[pivot_row] = tmp_p;
        }

        /* Compute multiplier and update submatrix */
        double inv_pivot = 1.0 / u_data[k * n + k];
        for (int i = k + 1; i < n; i++) {
            double factor = u_data[i * n + k] * inv_pivot;
            l_data[i * n + k] = factor;

            /* Update row i of U */
            double *row_i = u_data + i * n;
            double *row_k = u_data + k * n;
            for (int j = k; j < n; j++)
                row_i[j] -= factor * row_k[j];
        }
    }
    return 0;
}

static int forward_substitution(const matrix *restrict l,
                                const double *restrict b,
                                double *restrict y)
{
    int n = l->rows;
    const double *l_data = l->data;
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < i; j++)
            sum += l_data[i * n + j] * y[j];
        y[i] = b[i] - sum;
    }
    return 0;
}

static int backward_substitution(const matrix *restrict u,
                                 const double *restrict y,
                                 double *restrict x)
{
    int n = u->rows;
    const double *u_data = u->data;
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++)
            sum += u_data[i * n + j] * x[j];

        if (u_data[i * n + i] == 0.0) return -1;
        x[i] = (y[i] - sum) / u_data[i * n + i];
    }
    return 0;
}

int lu_solve(const matrix *restrict a,
             const double *restrict b,
             double *restrict x)
{
    if (!a || !b || !x) return -1;
    if (a->rows != a->cols) return -1;

    int n = a->rows;
    matrix *l = matrix_create(n, n);
    matrix *u = matrix_create(n, n);
    int *p = malloc(n * sizeof(int));
    if (!l || !u || !p) {
        matrix_free(l); matrix_free(u); free(p);
        return -1;
    }

    if (lu_decompose(a, l, u, p) != 0) {
        matrix_free(l); matrix_free(u); free(p);
        return -1;
    }

    double *bp = malloc(n * sizeof(double));
    double *y  = malloc(n * sizeof(double));
    if (!bp || !y) {
        free(bp); free(y);
        matrix_free(l); matrix_free(u); free(p);
        return -1;
    }

    /* Permute b */
    for (int i = 0; i < n; i++)
        bp[i] = b[p[i]];

    forward_substitution(l, bp, y);
    int ret = backward_substitution(u, y, x);

    free(bp); free(y);
    matrix_free(l); matrix_free(u); free(p);
    return ret;
}