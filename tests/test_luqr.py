import numpy as np
import time

# ----------------------------------------------------------------------
# Helper: fill a matrix with random values in [0, 1)
# ----------------------------------------------------------------------
def matrix_fill_random(m, n):
    return np.random.rand(m, n)

# ----------------------------------------------------------------------
# Helper: compute residual norm ||Ax - b||
# ----------------------------------------------------------------------
def residual_norm(A, x, b):
    return np.linalg.norm(A @ x - b)

# ----------------------------------------------------------------------
# Helper: check orthogonality of Q (Q^T Q should be I)
# ----------------------------------------------------------------------
def orthogonality_error(Q):
    # Q may be (m, n) with n <= m; check Q^T Q ≈ I_n
    Q_T_Q = Q.T @ Q
    n = Q_T_Q.shape[0]
    expected = np.eye(n)
    return np.max(np.abs(Q_T_Q - expected))

# ----------------------------------------------------------------------
# Helper: check that R is upper triangular (strictly lower part zero)
# ----------------------------------------------------------------------
def upper_triangular_error(R):
    m, n = R.shape
    # Extract indices where row > col (strictly lower part)
    rows, cols = np.where(np.arange(m)[:, None] > np.arange(n))
    if len(rows) == 0:
        return 0.0
    return np.max(np.abs(R[rows, cols]))

# ----------------------------------------------------------------------
# LU solver using numpy.linalg.solve (which internally uses LU)
# ----------------------------------------------------------------------
def lu_solve(A, b):
    return np.linalg.solve(A, b)

# ----------------------------------------------------------------------
# QR solver: use reduced QR decomposition to solve Ax = b (m >= n)
# ----------------------------------------------------------------------
def qr_solve(A, b):
    Q, R = np.linalg.qr(A, mode='reduced')  # Q (m,n), R (n,n)
    # Solve R x = Q^T b
    x = np.linalg.solve(R, Q.T @ b)
    return x

# ----------------------------------------------------------------------
# QR decomposition verification: returns Q, R, and errors
# ----------------------------------------------------------------------
def qr_decompose(A):
    Q, R = np.linalg.qr(A, mode='complete')  # Q (m,m), R (m,n)
    return Q, R

# ----------------------------------------------------------------------
# Test LU decomposition and solver
# ----------------------------------------------------------------------
def test_lu(n):
    print(f"\n========== LU Test (n = {n}) ==========")

    # Create matrix A and vector b (A * x = b, with x_exact = all ones)
    A = matrix_fill_random(n, n)
    x_exact = np.ones(n)
    b = A @ x_exact

    # Solve using LU (via numpy.linalg.solve)
    start = time.perf_counter()
    x_computed = lu_solve(A, b)
    elapsed = (time.perf_counter() - start) * 1000  # ms

    # Compute residual
    resid = residual_norm(A, x_computed, b)
    print(f"LU solve time: {elapsed:.3f} ms")
    print(f"Residual norm: {resid:.2e}")

    # Check error against exact solution
    max_err = np.max(np.abs(x_computed - x_exact))
    print(f"Max error in solution: {max_err:.2e}")

    return resid < 1e-10 and max_err < 1e-10

# ----------------------------------------------------------------------
# Test QR decomposition and solver (overdetermined system)
# ----------------------------------------------------------------------
def test_qr(m, n):
    print(f"\n========== QR Test (m={m}, n={n}) ==========")

    # Create matrix A (m x n) and vector b = A * x_exact
    A = matrix_fill_random(m, n)
    x_exact = np.ones(n)
    b = A @ x_exact

    # Solve using QR
    start = time.perf_counter()
    x_computed = qr_solve(A, b)
    elapsed = (time.perf_counter() - start) * 1000

    # Compute residual
    resid = residual_norm(A, x_computed, b)
    print(f"QR solve time: {elapsed:.3f} ms")
    print(f"Residual norm: {resid:.2e}")

    # Check error against exact solution
    max_err = np.max(np.abs(x_computed - x_exact))
    print(f"Max error in solution: {max_err:.2e}")

    # Additionally, compute full QR decomposition and verify properties
    Q, R = qr_decompose(A)

    # Check orthogonality of Q (Q^T Q ≈ I)
    ortho_err = orthogonality_error(Q)
    print(f"Orthogonality error (Q^T Q - I): {ortho_err:.2e}")

    # Check that R is upper triangular
    tri_err = upper_triangular_error(R)
    print(f"Upper triangular error (non-zero below diag): {tri_err:.2e}")

    # Check that Q * R ≈ A
    QR = Q @ R
    prod_err = np.max(np.abs(QR - A))
    print(f"Max error in Q*R - A: {prod_err:.2e}")

    return resid < 1e-10 and max_err < 1e-10

# ----------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------
def main():
    np.random.seed(42)   # reproducible results

    # Test LU for small square matrices
    test_lu(5)
    test_lu(100)
    test_lu(500)   # adjust size based on your machine's memory/time

    # Test QR for square and rectangular cases
    test_qr(5, 5)
    test_qr(100, 50)
    test_qr(200, 100)

if __name__ == "__main__":
    main()