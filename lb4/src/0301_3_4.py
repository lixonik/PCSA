import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('results.csv', header=None, names=["Size", "Threads", "Time"])

plt.figure(figsize=(10, 6))

for threads in [1, 2, 4, 8]:
    subset = data[data["Threads"] == threads]
    plt.plot(subset["Size"], subset["Time"], label=f'Количество потоков = {threads}')

plt.xlabel('Размерность СЛАУ')
plt.ylabel('Время (секунды)')
plt.title('Время решения СЛАУ методом Гаусса-Жордана')
plt.legend()
plt.grid(True)
plt.show()