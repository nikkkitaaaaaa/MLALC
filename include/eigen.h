#ifndef EIGEN_H
#define EIGEN_H

#include "matrix.h"

/* Power Method
   Finds dominant eigenvalue and eigenvector of square matrix A.

   a : input matrix (n x n)
   eigenvalue : output dominant eigenvalue
   eigenvector : output vector (size n)
   max_iter : max iterations
   tol : tolerance

   returns 0 on success, -1 on failure
*/
int power_method(const matrix *a, double *eigenvalue, double *eigenvector, int max_iter, double tol);

#endif
