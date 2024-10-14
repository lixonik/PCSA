import pandas as pd
import matplotlib.pyplot as plt

process_counts = [1, 2, 3, 4]

for number in process_counts:
  df = pd.read_csv(f'./measurements/measurements_{number}.csv')

  plt.plot(df['size'], df['sequential_time'], label='Последовательная сортировка выбором')
  plt.plot(df['size'], df['parallel_time'], label=f'Параллельная сортировка выбором (количество потоков = {number})')
  

  plt.xlabel('Размер массива')
  plt.ylabel('Время выполнения (секунды)')
  plt.title(f'Зависимость времени выполнения алгоритма\n от размера массива')
  plt.legend(loc='upper left')
  plt.grid(True)
  plt.savefig(f'./plots/plot_{number}.png')
  plt.close()