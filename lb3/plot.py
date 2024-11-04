import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('gauss_seidel_results.csv')

for threads in data['Threads'].unique():
    subset = data[data['Threads'] == threads]
    plt.plot(subset['Size'], subset['SerialTime'], label=f'Serial - {threads} threads')
    plt.plot(subset['Size'], subset['ParallelTime'], label=f'Parallel - {threads} threads')

plt.xlabel('Размерность СЛАУ')
plt.ylabel('Время (секунды)')
plt.title('Время решения СЛАУ методом Зейделя')
plt.legend()
plt.xscale('log')
plt.yscale('log')
plt.grid(True)
plt.savefig('gauss_seidel.png')
