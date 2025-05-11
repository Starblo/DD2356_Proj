import matplotlib.pyplot as plt

# New data
processes = [2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32]
parallel_times = [
    19.731658, 12.338776, 9.935904, 8.612729, 7.936014, 7.584225, 7.179784,
    6.866418, 6.617295, 6.551685, 6.484237, 6.337010, 6.390466, 6.175993,
    6.223011, 6.075932
]
serial_time = 33.501373

# Speedup and efficiency
speedup = [serial_time / t for t in parallel_times]
efficiency = [s / p for s, p in zip(speedup, processes)]

# Plotting
fig, axs = plt.subplots(1, 3, figsize=(18, 5))

# Time plot
axs[0].plot(processes, parallel_times, marker='o', color='b', label='Parallel Execution Time')
axs[0].axhline(y=serial_time, color='r', linestyle='--', label='Serial Execution Time')
axs[0].set_xlabel('Number of Processes')
axs[0].set_ylabel('Elapsed Time (seconds)')
axs[0].set_title('Elapsed Time vs Number of Processes')
axs[0].grid(True)
axs[0].legend()

# Speedup plot
axs[1].plot(processes, speedup, marker='o', color='g', label='Speedup')
axs[1].set_xlabel('Number of Processes')
axs[1].set_ylabel('Speedup')
axs[1].set_title('Speedup vs Number of Processes')
axs[1].grid(True)
axs[1].legend()

# Efficiency plot
axs[2].plot(processes, efficiency, marker='o', color='orange', label='Efficiency')
axs[2].set_xlabel('Number of Processes')
axs[2].set_ylabel('Efficiency')
axs[2].set_title('Efficiency vs Number of Processes')
axs[2].grid(True)
axs[2].legend()

plt.tight_layout()
plt.savefig("bonus_data_plot.png", dpi=300)
