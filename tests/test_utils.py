import numpy as np
import time

n = 2000

print(f"Testing with {n}×{n} matrix")

A = np.random.rand(n,n)

# COPY

start = time.perf_counter()
B = A.copy()
end = time.perf_counter()

print("NumPy copy time:", (end-start)*1000, "ms")

# TRANSPOSE

start = time.perf_counter()
T = A.T
end = time.perf_counter()

print("NumPy transpose time:", (end-start)*1000, "ms")