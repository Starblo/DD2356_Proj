import numpy as np
import matplotlib.pyplot as plt

threads = [1, 32, 64, 128]
results = {
    '1': [33976.5, 33967.4, 33971.5, 33974.1, 33977.2],
    '32': [315603.4, 317815.5, 317795.4, 316979.9, 316810.3],
    '64': [306549.8, 303088.6, 282754.1, 307594.3, 305188.3],
    '128': [273070.4, 273222.3, 276977.4, 273300.2, 276081.1]
}


schedules = ['static', 'dynamic', 'guided']
schedules_results = {
    'static': [
        277122.3, 274100.2, 275495.5, 281225.6, 251466.5
    ],
    'dynamic': [
        281115.6, 285492.7, 272246.9, 275292.1, 278679.7
    ],
    'guided': [
        280896.0, 276160.6, 277906.5, 279410.7, 278942.3
    ]
}




averages = {key: np.mean(value) for key, value in results.items()}
std_devs = {key: np.std(value) for key, value in results.items()}

schedules_averages = {key: np.mean(value) for key, value in schedules_results.items()}
schedules_std_devs = {key: np.std(value) for key, value in schedules_results.items()}

# Plotting
plt.figure(figsize=(10, 6))
plt.errorbar(threads, list(averages.values()), yerr=list(std_devs.values()), fmt='*', capsize=5)
plt.xticks(threads)
plt.xlabel('Number of Threads')
plt.ylabel('Bandwidth (MB/s)')
plt.title('Bandwidth vs Number of Threads')
plt.grid()
plt.savefig('stream.png')
plt.show()

# Plotting schedules
plt.figure(figsize=(10, 6))
plt.errorbar(schedules, list(schedules_averages.values()), yerr=list(schedules_std_devs.values()), fmt='d', capsize=5)
plt.xticks(schedules)
plt.xlabel('Scheduling Type')
plt.ylabel('Bandwidth (MB/s)')
plt.title('Bandwidth vs Scheduling Type')
plt.grid()
plt.savefig('scheduling.png')
plt.show()

