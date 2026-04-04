

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix.h"
#include "matrix_utils.h"
#include "LU.h"
#include "QR.h"

void print_vector(double *v, int n)
{
    for (int i = 0; i < n; i++) {
        printf("x[%d] = %.10lf\n", i, v[i]);
    }
}

int main(int argc, char *argv[])
{
    clock_t total_start = clock();

    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        printf("Example: %s system.txt\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    printf("Linear Algebra Solver Tool (LU / QR)\n");
    printf("------------------------------------\n");
    printf("Input file: %s\n\n", filename);

    /* ---------------- FILE READ TIMER ---------------- */

    clock_t load_start = clock();

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }

    int rows, cols;
    fscanf(fp, "%d %d", &rows, &cols);

    printf("Matrix size detected: %d x %d\n", rows, cols);

    matrix *A = matrix_create(rows, cols);

    /* Read matrix elements directly into A->data */
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(fp, "%lf", &A->data[i * cols + j]);
        }
    }

    double *b = (double *)malloc(sizeof(double) * rows);

    for (int i = 0; i < rows; i++) {
        fscanf(fp, "%lf", &b[i]);
    }

    fclose(fp);

    clock_t load_end = clock();
    double load_time = (double)(load_end - load_start) / CLOCKS_PER_SEC;

    printf("\nLoaded matrix A and vector b successfully.\n");

    double *x = (double *)malloc(sizeof(double) * cols);

    /* ---------------- SOLVER TIMER ---------------- */

    clock_t solver_start = clock();

    int status = 0;

    if (rows == cols) {
        printf("\nSystem type: Square system (Exact solve)\n");
        printf("Method selected: LU decomposition\n");

        status = lu_solve(A, b, x);
    }
    else if (rows > cols) {
        printf("\nSystem type: Overdetermined system (Least squares)\n");
        printf("Method selected: QR decomposition\n");

        status = qr_solve(A, b, x);
    }
    else {
        printf("\nSystem type: Underdetermined system\n");
        printf("Not supported.\n");
        return 0;
    }

    clock_t solver_end = clock();
    double solver_time = (double)(solver_end - solver_start) / CLOCKS_PER_SEC;

    /* ---------------- TOTAL TIMER ---------------- */

    clock_t total_end = clock();
    double total_time = (double)(total_end - total_start) / CLOCKS_PER_SEC;

    if (status != 0) {
        printf("\nSolver failed.\n");
    } 
    else {

        printf("\nSolution Vector x:\n");
        printf("------------------\n");

        print_vector(x, cols);

        printf("\nTiming Breakdown\n");
        printf("----------------\n");
        printf("File Load Time : %.6f seconds\n", load_time);
        printf("Solver Time    : %.6f seconds\n", solver_time);
        printf("Total Time     : %.6f seconds\n", total_time);
    }

    free(b);
    free(x);
    matrix_free(A);

    return 0;
}