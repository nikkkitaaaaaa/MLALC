#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "QR.h"
#include "matrix_utils.h"

/* Norm of a vector (plain loop) */
static double vector_norm(const double *restrict v, int n)
{
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += v[i] * v[i];
    return sqrt(sum);
}

int qr_decompose(const matrix *restrict a,
                 matrix *restrict q,
                 matrix *restrict r)
{
    if (!a || !q || !r) return -1;

    int m = a->rows;
    int n = a->cols;

    if (q->rows != m || q->cols != m) return -1;
    if (r->rows != m || r->cols != n) return -1;

    /* Initialize Q = I, R = A */
    matrix_fill_identity(q);
    memcpy(r->data, a->data, m * n * sizeof(double));

    double *r_data = r->data;
    double *q_data = q->data;

    int kmax = (m < n) ? m : n;

    /* Pre‑allocate temporary vectors (size m, reused for each column) */
    double *x = malloc(m * sizeof(double));
    double *v = malloc(m * sizeof(double));
    if (!x || !v) {
        free(x); free(v);
        return -1;
    }

    for (int k = 0; k < kmax; k++) {
        int size = m - k;   /* length of the active part */

        /* Extract column k from R (starting at row k) */
        for (int i = 0; i < size; i++)
            x[i] = r_data[(k + i) * n + k];

        double normx = vector_norm(x, size);
        if (normx == 0.0) continue;   /* skip zero columns */

        double alpha = (x[0] >= 0) ? -normx : normx;

        /* v = x - alpha*e1 (but only first element differs) */
        for (int i = 0; i < size; i++)
            v[i] = x[i];
        v[0] -= alpha;

        double normv = vector_norm(v, size);
        if (normv == 0.0) continue;   /* should not happen */

        double inv_normv = 1.0 / normv;
        for (int i = 0; i < size; i++)
            v[i] *= inv_normv;        /* normalize v */

        /* Apply Householder to R (right side) */
        for (int j = k; j < n; j++) {
            double dot = 0.0;
            for (int i = 0; i < size; i++)
                dot += v[i] * r_data[(k + i) * n + j];
            double factor = 2.0 * dot;
            for (int i = 0; i < size; i++)
                r_data[(k + i) * n + j] -= factor * v[i];
        }

        /* Apply Householder to Q (left side) */
        for (int j = 0; j < m; j++) {
            double dot = 0.0;
            for (int i = 0; i < size; i++)
                dot += v[i] * q_data[(k + i) * m + j];
            double factor = 2.0 * dot;
            for (int i = 0; i < size; i++)
                q_data[(k + i) * m + j] -= factor * v[i];
        }
    }

    free(x);
    free(v);

    /* The algorithm produces Q such that Q^T A = R (with Q orthogonal).
       For solving we need Q^T, so we transpose Q here (in‑place).
       This is an O(m^2) operation; can be avoided by building Q^T directly,
       but for clarity we keep the transpose. */
    matrix *qt = matrix_transpose(q);
    if (!qt) return -1;
    memcpy(q_data, qt->data, m * m * sizeof(double));
    matrix_free(qt);

    return 0;
}

int qr_solve(const matrix *restrict a,
             const double *restrict b,
             double *restrict x)
{
    if (!a || !b || !x) return -1;

    int m = a->rows;
    int n = a->cols;

    matrix *q = matrix_create(m, m);
    matrix *r = matrix_create(m, n);
    if (!q || !r) {
        matrix_free(q); matrix_free(r);
        return -1;
    }

    if (qr_decompose(a, q, r) != 0) {
        matrix_free(q); matrix_free(r);
        return -1;
    }

    double *y = malloc(m * sizeof(double));
    if (!y) {
        matrix_free(q); matrix_free(r);
        return -1;
    }

    /* Compute y = Q^T * b (q now holds Q^T) */
    const double *q_data = q->data;
    for (int i = 0; i < m; i++) {
        double sum = 0.0;
        for (int j = 0; j < m; j++)
            sum += q_data[i * m + j] * b[j];
        y[i] = sum;
    }

    /* Back substitution on R (upper triangular) */
    const double *r_data = r->data;
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++)
            sum += r_data[i * n + j] * x[j];

        if (fabs(r_data[i * n + i]) < 1e-12) {
            free(y);
            matrix_free(q); matrix_free(r);
            return -1;
        }
        x[i] = (y[i] - sum) / r_data[i * n + i];
    }

    free(y);
    matrix_free(q);
    matrix_free(r);
    return 0;
}