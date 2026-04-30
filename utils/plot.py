import csv
import os
import math
import matplotlib.pyplot as plt

base_dir = os.path.dirname(os.path.abspath(__file__))
data_path = os.path.join(base_dir, "..", "build", "data.csv")

if not os.path.exists(data_path):
    data_path = "data.csv"

n_vals = []
naive_vals = []
fftw_c2c_vals = []
fftw_r2c_vals = []
fftw_c2c_norm = []
fftw_r2c_norm = []
naive_norm_n2 = []

try:
    with open(data_path, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            n_vals.append(int(row["n"]))
            naive_vals.append(float(row["naive"]))
            fftw_c2c_vals.append(float(row["fftw_c2c"]))
            fftw_r2c_vals.append(float(row["fftw_r2c"]))
            fftw_c2c_norm.append(float(row["fftw_c2c_over_nlogn"]))
            fftw_r2c_norm.append(float(row["fftw_r2c_over_nlogn"]))
            naive_norm_n2.append(float(row["naive_over_n2"]))
except FileNotFoundError:
    print(f"Error: {data_path} not found.")
    exit(1)

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

ax1.plot(n_vals, naive_vals, marker='o', label='Naive $O(n^2)$')
ax1.plot(n_vals, fftw_c2c_vals, marker='s', label='FFTW Complex')
ax1.plot(n_vals, fftw_r2c_vals, marker='^', label='FFTW Real')
ax1.set_xlabel("Input Size (n)")
ax1.set_ylabel("Time (seconds)")
ax1.set_title("Toeplitz Multiplication Benchmark")
ax1.legend()
ax1.grid(True, which="both", ls="-", alpha=0.5)
ax1.set_yscale("log") 

ax2.plot(n_vals, fftw_c2c_norm, marker='s', color='orange', label='FFTW Complex / ($n \log n$)')
ax2.plot(n_vals, fftw_r2c_norm, marker='^', color='green', label='FFTW Real / ($n \log n$)')
ax2.plot(n_vals, naive_norm_n2, marker='o', color='blue', label='Naive / $n^2$')
ax2.set_xlabel("Input Size (n)")
ax2.set_ylabel("Normalized Time (Constant Factor)")
ax2.set_title("nlogn Profile")
ax2.legend()
ax2.grid(True, which="both", ls="-", alpha=0.5)

plt.tight_layout()
plt.show()