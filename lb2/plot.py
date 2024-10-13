import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('times.csv')

plt.plot(df['size'], df['sequential_time'], label='Последовательная сортировка выбором')
plt.plot(df['size'], df['parallel_time'], label='Параллельная сортировка выбором')

plt.xlabel('Размер массива')
plt.ylabel('Время выполнения (секунды)')
plt.title('Зависимость времени выполнения от размерности цепочки данных')
plt.legend()
plt.grid(True)
plt.savefig('measurements.png')