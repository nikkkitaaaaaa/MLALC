#ifndef QR_H
#define QR_H

#include "matrix.h"

/**
 * Perform QR decomposition using Householder reflections.
 * 
 * @param a  Input matrix (m x n)
 * @param q  Output orthogonal matrix (m x m) (Q^T will be stored)
 * @param r  Output upper triangular matrix (m x n)
 * @return   0 on success, -1 on error
 */
int qr_decompose(const matrix *a, matrix *q, matrix *r);

/**
 * Solve the linear system A * x = b using QR decomposition.
 * 
 * @param a  Input matrix (m x n)
 * @param b  Right-hand side vector of size m
 * @param x  Output solution vector of size n
 * @return   0 on success, -1 on error
 */
int qr_solve(const matrix *a, const double *b, double *x);

#endif /* QR_H */