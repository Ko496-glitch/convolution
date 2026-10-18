
import csv
import os
import matplotlib.pyplot as plt

# -----------------------------------------
# Paths
# -----------------------------------------
base_dir = os.path.dirname(os.path.abspath(__file__))
data_path = os.path.join(base_dir, "..", "build", "data.csv")

if not os.path.exists(data_path):
    data_path = "data.csv"

# -----------------------------------------
# Toeplitz benchmark data
# -----------------------------------------
n_vals = []
naive_vals = []
fftw_c2c_vals = []
fftw_r2c_vals = []
ntl_fft_vals = []

fftw_c2c_norm = []
fftw_r2c_norm = []
naive_norm_n2 = []
ntl_fft_norm = []

try:
    with open(data_path, "r") as f:
        reader = csv.DictReader(f)

        for row in reader:
            n_vals.append(int(row["n"]))
            naive_vals.append(float(row["naive"]))
            fftw_c2c_vals.append(float(row["fftw_c2c"]))
            fftw_r2c_vals.append(float(row["fftw_r2c"]))
            ntl_fft_vals.append(float(row["ntl_fft"]))

            fftw_c2c_norm.append(
                float(row["fftw_c2c_over_nlogn"])
            )
            fftw_r2c_norm.append(
                float(row["fftw_r2c_over_nlogn"])
            )
            naive_norm_n2.append(
                float(row["naive_over_n2"])
            )
            ntl_fft_norm.append(
                float(row["ntl_fft_over_nlogn"])
            )

except FileNotFoundError:
    print(f"Error: {data_path} not found.")
    raise SystemExit(1)

# -----------------------------------------
# Butterfly hardware-counter results
# Apple Silicon M3, preliminary measurements
# -----------------------------------------
algorithms = [
    "Harvey Algorithm 2",
    "Harvey + Barrett",
]

cycles = [4.1069, 10.2060]
instructions = [27.3147, 56.3159]
nanoseconds = [1.8537, 4.6619]

# -----------------------------------------
# Dashboard UI
# -----------------------------------------
fig, axes = plt.subplots(2, 3, figsize=(18, 10))

fig.suptitle(
    "Convolution Research: Toeplitz & Butterfly Performance",
    fontsize=17,
    fontweight="bold"
)

# =========================================
# Plot 1: Toeplitz runtime
# =========================================
ax = axes[0, 0]

ax.plot(n_vals, naive_vals, marker="o",
        label="Naive O(n²)")
ax.plot(n_vals, fftw_c2c_vals, marker="s",
        label="FFTW Complex")
ax.plot(n_vals, fftw_r2c_vals, marker="^",
        label="FFTW Real")
ax.plot(n_vals, ntl_fft_vals, marker="x",
        label="NTL FFT")

ax.set_title("Toeplitz Multiplication")
ax.set_xlabel("Input Size (n)")
ax.set_ylabel("Time (seconds)")
ax.set_yscale("log")
ax.legend(fontsize=8)
ax.grid(True, alpha=0.3)

# =========================================
# Plot 2: Normalized complexity
# =========================================
ax = axes[0, 1]

ax.plot(n_vals, fftw_c2c_norm, marker="s",
        label="FFTW Complex / n log n")
ax.plot(n_vals, fftw_r2c_norm, marker="^",
        label="FFTW Real / n log n")
ax.plot(n_vals, naive_norm_n2, marker="o",
        label="Naive / n²")
ax.plot(n_vals, ntl_fft_norm, marker="x",
        label="NTL FFT / n log n")

ax.set_title("Normalized Complexity Profile")
ax.set_xlabel("Input Size (n)")
ax.set_ylabel("Normalized Time")
ax.legend(fontsize=8)
ax.grid(True, alpha=0.3)

# =========================================
# Plot 3: Cycles per butterfly
# =========================================
ax = axes[0, 2]

bars = ax.bar(
    algorithms,
    cycles,
    color=["steelblue", "darkorange"]
)

ax.set_title("CPU Cycles per Butterfly")
ax.set_ylabel("Cycles / butterfly")
ax.bar_label(bars, fmt="%.2f", padding=3)
ax.set_ylim(0, max(cycles) * 1.2)
ax.grid(axis="y", alpha=0.3)

# =========================================
# Plot 4: Instructions per butterfly
# =========================================
ax = axes[1, 0]

bars = ax.bar(
    algorithms,
    instructions,
    color=["steelblue", "darkorange"]
)

ax.set_title("Instructions per Butterfly")
ax.set_ylabel("Instructions / butterfly")
ax.bar_label(bars, fmt="%.2f", padding=3)
ax.set_ylim(0, max(instructions) * 1.2)
ax.grid(axis="y", alpha=0.3)

# =========================================
# Plot 5: Nanoseconds per butterfly
# =========================================
ax = axes[1, 1]

bars = ax.bar(
    algorithms,
    nanoseconds,
    color=["steelblue", "darkorange"]
)

ax.set_title("Execution Time per Butterfly")
ax.set_ylabel("Nanoseconds / butterfly")
ax.bar_label(bars, fmt="%.3f", padding=3)
ax.set_ylim(0, max(nanoseconds) * 1.2)
ax.grid(axis="y", alpha=0.3)

# =========================================
# Plot 6: Relative performance
# =========================================
ax = axes[1, 2]

speed_ratios = [
    1.0,
    cycles[1] / cycles[0],
]

bars = ax.bar(
    algorithms,
    speed_ratios,
    color=["steelblue", "darkorange"]
)

ax.set_title("Relative CPU Cycle Cost")
ax.set_ylabel("Cycles relative to Harvey")
ax.bar_label(bars, fmt="%.2fx", padding=3)
ax.set_ylim(0, max(speed_ratios) * 1.2)
ax.grid(axis="y", alpha=0.3)

# -----------------------------------------
# Layout
# -----------------------------------------
for ax in axes.flat:
    ax.tick_params(axis="x", labelsize=8)

plt.tight_layout(rect=[0, 0, 1, 0.95])
plt.show()
