import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('results.csv', header=None, names=["Size", "Threads", "Time"])

plt.figure(figsize=(10, 6))

for threads in [1, 2, 4, 8]:
    subset = data[data["Threads"] == threads]
    plt.plot(subset["Size"], subset["Time"], label=f'Threads={threads}')

plt.xlabel('Size of the system')
plt.ylabel('Time (seconds)')
plt.title('Time to solve Ax = b using Gauss-Jordan')
plt.legend()
plt.grid(True)
plt.show()