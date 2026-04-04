import numpy as np
import time
import csv

filename = "examples/datasets/PRSA_data_2010.1.1-2014.12.31.csv"

total_start = time.time()

print("Linear Algebra Solver Tool (Python)")
print("------------------------------------")
print("Input file: " + filename)
print("")

load_start = time.time()

A_list = []
b_list = []

with open(filename, "r") as f:

    reader = csv.reader(f)

    header = next(reader)   # skip header

    for row in reader:

        try:
            pm25 = float(row[5])
            dew  = float(row[6])
            temp = float(row[7])
            pres = float(row[8])

            # A matrix (features)
            A_list.append([dew, temp, pres])

            # b vector (target)
            b_list.append(pm25)

        except:
            continue   # skip rows with NA or invalid values

A = np.array(A_list)
b = np.array(b_list)

rows = A.shape[0]
cols = A.shape[1]

load_end = time.time()
load_time = load_end - load_start

print("Matrix size detected: %d x %d" % (rows, cols))
print("Loaded matrix A and vector b successfully.")

solver_start = time.time()

if rows > cols:

    print("")
    print("System type: Overdetermined system")
    print("Method: Least Squares")

    x = np.linalg.lstsq(A, b)[0]

else:

    print("")
    print("Square system")

    x = np.linalg.solve(A, b)

solver_end = time.time()
solver_time = solver_end - solver_start

total_end = time.time()
total_time = total_end - total_start

print("")
print("Solution Vector x")
print("------------------")

for i in range(len(x)):
    print("x[%d] = %.10f" % (i, x[i]))

print("")
print("Timing Breakdown")
print("----------------")
print("File Load Time : %.6f seconds" % load_time)
print("Solver Time    : %.6f seconds" % solver_time)
print("Total Time     : %.6f seconds" % total_time)