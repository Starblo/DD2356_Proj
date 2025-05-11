import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("row_sums_output.log")
plt.plot(data, marker='o', label='Row Sums')
plt.xlabel("Row Index")
plt.ylabel("Sum Value")
plt.title("Parallel Row Sum Computation")
plt.legend()
plt.savefig("row_sums_plot_parallel.png")