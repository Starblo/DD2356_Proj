import numpy as np
import matplotlib.pyplot as plt

# Example data (replace with actual measurements)
operational_intensity = np.array([0.0121])  # FLOPs per byte
performance = np.array([0.2905])  # GFLOP/s

# Set the memory bandwidth and peak performance values
memory_bandwidth = 141.5  # GB/s
peak_performance = 6963.2  # GFLOP/s

# Create the plot
plt.figure(figsize=(8,6))

# Plot measured data
plt.loglog(operational_intensity, performance, marker='o', label='Measured Data')

# Plot the peak performance line (horizontal line at peak performance)
plt.axhline(y=peak_performance, color='r', linestyle='--', label='Peak FLOP/s')

# Plot the memory bandwidth limit line (memory bandwidth as slope)
# Memory bandwidth limit line: y = memory_bandwidth * x (linear relationship)
# Since we are on a log-log scale, we need to adjust accordingly.
x_vals = np.logspace(-2, 2, 500)  # Generate logarithmic x-values
y_vals = memory_bandwidth * x_vals  # Calculate y-values for the memory bandwidth limit line

plt.plot(x_vals, y_vals, linestyle='dotted', color='gray', label='Memory Bandwidth Limit')

# Set labels and title
plt.xlabel('Operational Intensity (FLOPs/Byte)')
plt.ylabel('Performance (GFLOP/s)')
plt.title('Roofline Model')

# Add a legend and grid
plt.legend()
plt.grid(True, which='both', linestyle='--')

# Show the plot
plt.show()
