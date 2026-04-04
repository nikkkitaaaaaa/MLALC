#ifndef LU_H
#define LU_H

#include "matrix.h"

/**
 * Perform LU decomposition with partial pivoting.
 * 
 * @param a  Input square matrix (n x n)
 * @param l  Output lower triangular matrix (n x n)
 * @param u  Output upper triangular matrix (n x n)
 * @param p  Output permutation vector of size n
 * @return   0 on success, -1 on error
 */
int lu_decompose(const matrix *a, matrix *l, matrix *u, int *p);

/**
 * Solve the linear system A * x = b using LU decomposition.
 * 
 * @param a  Input square matrix (n x n)
 * @param b  Right-hand side vector of size n
 * @param x  Output solution vector of size n
 * @return   0 on success, -1 on error
 */
int lu_solve(const matrix *a, const double *b, double *x);

#endif /* LU_H */