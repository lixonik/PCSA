import pandas as pd
import matplotlib.pyplot as plt

csv_file = "results.csv"
data = pd.read_csv(csv_file)

sizes = data['Size'].unique()
threads = data['Threads'].unique()

plt.figure(figsize=(12, 8))

for t in threads:
    subset = data[data['Threads'] == t]
    plt.plot(subset['Size'], subset['Sequential'], label=f"Последовательный алгоритм (количество потоков = {t})", linestyle='--')
    plt.plot(subset['Size'], subset['Parallel'], label=f"Параллельный алгоритм (количество потоков = {t})")

plt.xscale('log')
plt.yscale('log')
plt.xlabel("Размерность СЛАУ")
plt.ylabel("Время выполнения, секунды")
plt.title("Решение СЛАУ методом разложения Холецкого")
plt.legend()
plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.tight_layout()
plt.savefig("execution_times.png")
plt.show()
