import matplotlib.pyplot as plt

serial_time = 0.361084

# Data: number of processes vs time taken
processes = [2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32]
times = [0.375145, 0.340984, 0.286463, 0.336919, 0.304556, 0.303978, 0.298379, 0.307919, 0.315343, 0.302134,
         0.299446, 0.320297, 0.325954, 0.317972, 0.315764, 0.345788]
         
plt.figure(figsize=(10, 6))
plt.plot(processes, times, marker='o', linestyle='-', color='b', label="Parallel Execution Time")
plt.axhline(y=serial_time, color='r', linestyle='--', label="Serial Execution Time (0.361084 s)")

# Add labels and title
plt.xlabel('Number of Processes', fontsize=12)
plt.ylabel('Total Time (seconds)', fontsize=12)
plt.title('Scalability Plot: Time vs Number of Processes', fontsize=14)

# Add grid and legend
plt.grid(True)
plt.legend()

# Add grid
plt.grid(True)

# Save the plot as an image
plt.savefig('exercise1_scalability.png')


# Calculate the speedup for each number of processes
speedup = [serial_time / time for time in times]

# Create a speedup plot
plt.figure(figsize=(10, 6))
plt.plot(processes, speedup, marker='o', linestyle='-', color='g', label="Speedup")

# Add labels and title
plt.xlabel('Number of Processes', fontsize=12)
plt.ylabel('Speedup', fontsize=12)
plt.title('Speedup Plot: Speedup vs Number of Processes', fontsize=14)

# Add grid and legend
plt.grid(True)
plt.legend()

plt.savefig('exercise1_speedup.png')


nodes = [2, 4, 5, 8]
times_nodes = [0.341299, 0.297673, 0.350066, 0.405029]

# Create the time plot for nodes
plt.figure(figsize=(10, 6))
plt.plot(nodes, times_nodes, marker='o', linestyle='-', color='b', label="Parallel Execution Time")

# Add labels and title
plt.xlabel('Number of Nodes', fontsize=12)
plt.ylabel('Total Time (seconds)', fontsize=12)
plt.title('Time vs Number of Nodes', fontsize=14)

# Add grid and legend
plt.grid(True)
plt.legend()
plt.savefig('exercise1_nodes.png')


efficiency = [s / p for s, p in zip(speedup, processes)]

# Plotting efficiency
plt.figure(figsize=(10, 6))
plt.plot(processes, efficiency, marker='o', linestyle='-', color='orange', label="Efficiency")

plt.xlabel('Number of Processes', fontsize=12)
plt.ylabel('Efficiency', fontsize=12)
plt.title('Efficiency Plot: Efficiency vs Number of Processes', fontsize=14)
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig('exercise1_efficiency.png')