import csv
import os
import matplotlib.pyplot as plt

n_vals = []
naive_vals = []
fftw_c2c_vals = []
fftw_r2c_vals = []

base_dir = os.path.dirname(os.path.abspath(__file__))
data_path = os.path.join(base_dir, "..","build", "data.csv")

with open(data_path, "r") as f:
    reader = csv.DictReader(f)
    for row in reader:
        n_vals.append(int(row["n"]))
        naive_vals.append(float(row["naive"]))
        fftw_c2c_vals.append(float(row["fftw_c2c"]))
        fftw_r2c_vals.append(float(row["fftw_r2c"]))

plt.figure()

plt.plot(n_vals, naive_vals, marker='o', label='Naive O(n^2)')
plt.plot(n_vals, fftw_c2c_vals, marker='o', label='FFTW Complex')
plt.plot(n_vals, fftw_r2c_vals, marker='o', label='FFTW Real')

plt.xlabel("n")
plt.ylabel("time (seconds)")
plt.title("Toeplitz Multiplication Benchmark")


fft_norm = [t / (n * math.log2(n)) for t, n in zip(fftw_c2c_vals, n_vals)]
naive_norm = [t / (n*n) for t, n in zip(naive_vals, n_vals)]

plt.figure()
plt.plot(n_vals, fft_norm, marker='o', label='FFTW / (n log n)')
plt.plot(n_vals, naive_norm, marker='o', label='Naive / n^2')

plt.xlabel("n")
plt.ylabel("normalized time")
plt.title("Complexity Verification")



plt.legend()
plt.grid(True)
plt.yscale("log")  
plt.show()