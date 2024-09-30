import matplotlib.pyplot as plt
import csv

data = {}

with open('matrix_multiply.csv', 'r') as file:
    reader = csv.DictReader(file)
    for row in reader:
        size = int(row['MatrixSize'])
        threads = int(row['Threads'])
        if threads not in data:
            data[threads] = {'sizes': [], 'serial': [], 'parallel_for': [], 'parallel_manual': []}
        data[threads]['sizes'].append(size)
        data[threads]['serial'].append(float(row['Serial']))
        data[threads]['parallel_for'].append(float(row['ParallelFor']))
        data[threads]['parallel_manual'].append(float(row['ParallelManual']))

for threads, results in data.items():
    plt.figure()
    plt.plot(results['sizes'], results['serial'], label='Serial')
    plt.plot(results['sizes'], results['parallel_for'], label='Parallel For')
    plt.plot(results['sizes'], results['parallel_manual'], label='Parallel Manual')
    plt.xlabel('Matrix Size')
    plt.ylabel('Time (seconds)')
    plt.title(f'Matrix Multiply Performance with {threads} Threads')
    plt.legend()
    plt.grid(True)
    plt.xscale('log')
    plt.yscale('log')
    plt.savefig(f'./plots/matrix_multiply_{threads}.png')
