import matplotlib.pyplot as plt

# Data: number of processes vs time taken
processes = [2, 4, 5, 8, 10, 20, 25, 32]
times = [12.645312, 10.861026, 11.440455, 10.681691, 10.249588, 9.470116, 10.380918, 10.295008]
compute_time = [2.672311, 1.434711, 1.072636, 0.665275, 0.530615, 0.265109, 0.213873, 0.206727]

# Serial time
serial_time = 12.000776

# Calculate the speedup for each number of processes
speedup = [serial_time / time for time in times]

# Create the figure and subplots
fig, axs = plt.subplots(1, 2, figsize=(14, 6))

# Time plot
axs[0].plot(processes, times, marker='o', linestyle='-', color='b', label="Parallel Execution Time")
axs[0].axhline(y=serial_time, color='r', linestyle='--', label="Serial Execution Time")
axs[0].set_xlabel('Number of Processes', fontsize=12)
axs[0].set_ylabel('Total Time (seconds)', fontsize=12)
axs[0].set_title('Time vs Number of Processes', fontsize=14)
axs[0].grid(True)
axs[0].legend()

# Speedup plot
axs[1].plot(processes, speedup, marker='o', linestyle='-', color='g', label="Speedup")
axs[1].set_xlabel('Number of Processes', fontsize=12)
axs[1].set_ylabel('Speedup', fontsize=12)
axs[1].set_title('Speedup vs Number of Processes', fontsize=14)
axs[1].grid(True)
axs[1].legend()

# Save the plot as an image
plt.tight_layout()
plt.savefig('exercise2_scalability_speedup.png')

import matplotlib.pyplot as plt

nodes = [2, 4, 5, 8]
elapsed_time = [10.703456, 9.768310, 9.429867, 9.081685]


plt.figure(figsize=(8, 6))
plt.plot(nodes, elapsed_time, marker='o', linestyle='-', color='b')
plt.title('Elapsed Time vs Number of Nodes')
plt.xlabel('Number of Nodes')
plt.ylabel('Elapsed Time (seconds)')
plt.grid(True)
plt.xticks(nodes)

plt.savefig('exercise2_scalability_nodes.png')

# plot compute time and elapsed time
plt.figure(figsize=(8, 6))
plt.plot(processes, compute_time, marker='o', linestyle='-', color='g', label='Compute Time')
plt.plot(processes, times, marker='o', linestyle='-', color='b', label='Total Execution Time')
plt.title('Compute Time and Total Execution Time vs Number of Processes')
plt.xlabel('Number of Processes')
plt.ylabel('Time (seconds)')
plt.grid(True)
plt.legend()
plt.savefig('exercise2_compute_time.png')

# Data
processes = [2, 4, 5, 8, 10, 20, 25]
times = [0.061967, 0.189872, 0.376514, 0.913826, 1.392187, 5.204246, 9.763372]
serial_time = 0.017356


plt.figure(figsize=(8, 6))
plt.plot(processes, times, marker='o', color='blue', label='Parallel Execution Time')
plt.axhline(y=serial_time, color='red', linestyle='--', label='Serial Execution Time')
plt.xlabel('Number of Processes')
plt.ylabel('Elapsed Time (seconds)')
plt.title('Execution Time vs Number of Processes')
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig('exercise2_weak_sca_plot.png')