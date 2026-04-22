import csv
import matplotlib.pyplot as plt

n_vals = []
time_vals = []

with open("build/data.csv", "r") as f:
    reader = csv.DictReader(f)
    for row in reader:
        n_vals.append(int(row["n"]))
        time_vals.append(float(row["time"]))

plt.plot(n_vals, time_vals, marker="o")
plt.xlabel("n")
plt.ylabel("time (seconds)")
plt.title("Toeplitz Benchmark")
plt.grid(True)
plt.show()