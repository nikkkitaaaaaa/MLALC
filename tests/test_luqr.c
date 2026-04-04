// test_lu_qr.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "matrix_utils.h"
#include "LU.h"
#include "QR.h"

// ----------------------------------------------------------------------
// Helper: fill a matrix with random values in [0,1)
// ----------------------------------------------------------------------
void matrix_fill_random(matrix *m) {
    if (!m) return;
    int total = m->rows * m->cols;
    for (int i = 0; i < total; i++) {
        m->data[i] = (double)rand() / RAND_MAX;
    }
}

// ----------------------------------------------------------------------
// Helper: compute residual norm ||Ax - b||
// ----------------------------------------------------------------------
double residual_norm(const matrix *A, const double *x, const double *b) {
    int m = A->rows;
    int n = A->cols;
    double sum_sq = 0.0;
    for (int i = 0; i < m; i++) {
        double Ax_i = 0.0;
        for (int j = 0; j < n; j++) {
            Ax_i += A->data[i * n + j] * x[j];
        }
        double diff = Ax_i - b[i];
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq);
}

// ----------------------------------------------------------------------
// Helper: check orthogonality of Q (Q^T Q should be I)
// ----------------------------------------------------------------------
double orthogonality_error(const matrix *Q) {
    int n = Q->rows;
    double max_err = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double dot = 0.0;
            for (int k = 0; k < n; k++) {
                dot += Q->data[k * n + i] * Q->data[k * n + j];
            }
            double expected = (i == j) ? 1.0 : 0.0;
            double err = fabs(dot - expected);
            if (err > max_err) max_err = err;
        }
    }
    return max_err;
}

// ----------------------------------------------------------------------
// Helper: check that R is upper triangular (strictly lower part zero)
// ----------------------------------------------------------------------
double upper_triangular_error(const matrix *R) {
    int m = R->rows;
    int n = R->cols;
    double max_err = 0.0;
    for (int i = 1; i < m; i++) {
        for (int j = 0; j < i && j < n; j++) {
            double err = fabs(R->data[i * n + j]);
            if (err > max_err) max_err = err;
        }
    }
    return max_err;
}

// ----------------------------------------------------------------------
// Timing utility (milliseconds)
// ----------------------------------------------------------------------
double get_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1.0e6;
}

// ----------------------------------------------------------------------
// Test LU decomposition and solver
// ----------------------------------------------------------------------
int test_lu(int n) {
    printf("\n========== LU Test (n = %d) ==========\n", n);

    // Create matrix A and vector b (A * x = b, with known solution x_exact = all ones)
    matrix *A = matrix_create(n, n);
    if (!A) { printf("Failed to create A\n"); return -1; }
    matrix_fill_random(A);

    double *x_exact = malloc(n * sizeof(double));
    double *b = malloc(n * sizeof(double));
    double *x_computed = malloc(n * sizeof(double));
    if (!x_exact || !b || !x_computed) {
        printf("Memory allocation failed\n");
        matrix_free(A);
        free(x_exact); free(b); free(x_computed);
        return -1;
    }

    // Set x_exact = all ones, compute b = A * x_exact
    for (int i = 0; i < n; i++) x_exact[i] = 1.0;
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++)
            sum += A->data[i * n + j] * x_exact[j];
        b[i] = sum;
    }

    // Solve using LU
    double start = get_time_ms();
    int ret = lu_solve(A, b, x_computed);
    double elapsed = get_time_ms() - start;

    if (ret != 0) {
        printf("LU solve failed (singular matrix?)\n");
        matrix_free(A);
        free(x_exact); free(b); free(x_computed);
        return -1;
    }

    // Compute residual
    double resid = residual_norm(A, x_computed, b);
    printf("LU solve time: %.3f ms\n", elapsed);
    printf("Residual norm: %.2e\n", resid);

    // Check error against exact solution
    double max_err = 0.0;
    for (int i = 0; i < n; i++) {
        double err = fabs(x_computed[i] - x_exact[i]);
        if (err > max_err) max_err = err;
    }
    printf("Max error in solution: %.2e\n", max_err);

    matrix_free(A);
    free(x_exact); free(b); free(x_computed);
    return (resid < 1e-10 && max_err < 1e-10) ? 0 : -1;
}

// ----------------------------------------------------------------------
// Test QR decomposition and solver (overdetermined system)
// ----------------------------------------------------------------------
int test_qr(int m, int n) {
    printf("\n========== QR Test (m=%d, n=%d) ==========\n", m, n);

    // Create matrix A (m x n) and vector b = A * x_exact
    matrix *A = matrix_create(m, n);
    if (!A) { printf("Failed to create A\n"); return -1; }
    matrix_fill_random(A);

    double *x_exact = malloc(n * sizeof(double));
    double *b = malloc(m * sizeof(double));
    double *x_computed = malloc(n * sizeof(double));
    if (!x_exact || !b || !x_computed) {
        printf("Memory allocation failed\n");
        matrix_free(A);
        free(x_exact); free(b); free(x_computed);
        return -1;
    }

    // Exact solution: all ones
    for (int i = 0; i < n; i++) x_exact[i] = 1.0;
    // Compute b = A * x_exact
    for (int i = 0; i < m; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++)
            sum += A->data[i * n + j] * x_exact[j];
        b[i] = sum;
    }

    // Solve using QR
    double start = get_time_ms();
    int ret = qr_solve(A, b, x_computed);
    double elapsed = get_time_ms() - start;

    if (ret != 0) {
        printf("QR solve failed\n");
        matrix_free(A);
        free(x_exact); free(b); free(x_computed);
        return -1;
    }

    // Compute residual
    double resid = residual_norm(A, x_computed, b);
    printf("QR solve time: %.3f ms\n", elapsed);
    printf("Residual norm: %.2e\n", resid);

    // Check error against exact solution
    double max_err = 0.0;
    for (int i = 0; i < n; i++) {
        double err = fabs(x_computed[i] - x_exact[i]);
        if (err > max_err) max_err = err;
    }
    printf("Max error in solution: %.2e\n", max_err);

    // Additionally, compute QR decomposition and verify properties
    matrix *Q = matrix_create(m, m);
    matrix *R = matrix_create(m, n);
    if (!Q || !R) {
        printf("Failed to create Q/R for verification\n");
        matrix_free(A); free(x_exact); free(b); free(x_computed);
        if (Q) matrix_free(Q); if (R) matrix_free(R);
        return -1;
    }

    ret = qr_decompose(A, Q, R);
    if (ret != 0) {
        printf("QR decomposition failed\n");
    } else {
        // Check orthogonality of Q (Q^T Q ≈ I)
        double ortho_err = orthogonality_error(Q);
        printf("Orthogonality error (Q^T Q - I): %.2e\n", ortho_err);

        // Check that R is upper triangular
        double tri_err = upper_triangular_error(R);
        printf("Upper triangular error (non-zero below diag): %.2e\n", tri_err);

        // Check that Q * R ≈ A
        double prod_err = 0.0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                double sum = 0.0;
                for (int k = 0; k < m; k++)
                    sum += Q->data[i * m + k] * R->data[k * n + j];
                double diff = fabs(sum - A->data[i * n + j]);
                if (diff > prod_err) prod_err = diff;
            }
        }
        printf("Max error in Q*R - A: %.2e\n", prod_err);
    }

    matrix_free(A);
    matrix_free(Q);
    matrix_free(R);
    free(x_exact); free(b); free(x_computed);
    return (resid < 1e-10 && max_err < 1e-10) ? 0 : -1;
}

// ----------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------
int main() {
    srand(42);   // for reproducible results

    // Test LU for small square matrices
    test_lu(5);
    test_lu(100);
    test_lu(500);   // adjust size based on your machine's memory/time

    // Test QR for square and rectangular cases
    test_qr(5, 5);
    test_qr(100, 50);
    test_qr(200, 100);

    return 0;
}