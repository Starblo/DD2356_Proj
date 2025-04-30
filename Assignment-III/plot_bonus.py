import matplotlib.pyplot as plt

neurons = [500, 1000, 1500, 2000, 2500, 3000]
times = [0.717642, 0.936454, 1.361631, 1.754526, 2.386446, 2.839078]

plt.figure()
plt.plot(neurons, times, marker='o')
plt.xlabel('Number of neurons')
plt.ylabel('Simulation time (seconds)')
plt.title('Simulation Time vs. Number of Neurons')
plt.savefig("bonus_time_neurons.png")

thresholds = [30, 40, 50, 60, 70, 80, 90, 100]
times = [0.880779, 0.866585, 0.875839, 0.782630, 0.511036, 0.864868, 0.844965, 0.921650]

plt.figure()
plt.plot(thresholds, times, marker='*')
plt.ylim(bottom=0)
plt.xlabel('Threshold')
plt.ylabel('Simulation time (seconds)')
plt.title('Simulation Time vs. Threshold')
plt.savefig("bonus_time_threshold.png")