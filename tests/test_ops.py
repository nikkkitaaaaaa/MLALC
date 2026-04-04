import numpy as np
import time

n = 1000

print(f"Testing with {n}x{n} matrix")

A = np.random.rand(n,n)
B = np.random.rand(n,n)

# ADD
start = time.perf_counter()
C = A + B
end = time.perf_counter()

print("NumPy add time:", (end-start)*1000, "ms")

# SUBTRACT
start = time.perf_counter()
C = A - B
end = time.perf_counter()

print("NumPy subtract time:", (end-start)*1000, "ms")

# SCALAR MULTIPLY
start = time.perf_counter()
C = A * 2.5
end = time.perf_counter()

print("NumPy scalar multiply time:", (end-start)*1000, "ms")

# MATRIX MULTIPLY
start = time.perf_counter()
C = A @ B
end = time.perf_counter()

print("NumPy matrix multiply time:", (end-start)*1000, "ms")