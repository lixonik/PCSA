#include <omp.h>
#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

const double TOLERANCE = 1e-10;  // критерий сходимости
const int MAX_ITER = 10000;  // максимальное количество итераций

template <typename Function>
double measure_time(Function fn) {
   auto start = std::chrono::high_resolution_clock::now();
   fn();
   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> duration = end - start;
   return duration.count();
}

std::vector<std::vector<double>> generate_random_matrix(int size) {
   std::vector<std::vector<double>> matrix(size, std::vector<double>(size + 1));
   for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
         matrix[i][j] =
             (i == j) ? 1.0 + size : static_cast<double>(rand()) / RAND_MAX;
      }
      matrix[i][size] = static_cast<double>(rand()) / RAND_MAX;  // вектор b
   }
   return matrix;
}

bool gauss_seidel_serial(const std::vector<std::vector<double>> &matrix,
                         std::vector<double> &x) {
   int size = matrix.size();
   std::vector<double> prev_x(size, 0.0);
   for (int iter = 0; iter < MAX_ITER; ++iter) {
      for (int i = 0; i < size; ++i) {
         double sum = matrix[i][size];
         for (int j = 0; j < size; ++j) {
            if (i != j) sum -= matrix[i][j] * x[j];
         }
         x[i] = sum / matrix[i][i];
      }
      double error = 0.0;
      for (int i = 0; i < size; ++i) error += std::abs(x[i] - prev_x[i]);
      if (error < TOLERANCE) return true;
      prev_x = x;
   }
   return false;
}

bool gauss_seidel_parallel(const std::vector<std::vector<double>> &matrix,
                           std::vector<double> &x, int num_threads) {
   int size = matrix.size();
   std::vector<double> prev_x(size, 0.0);
   for (int iter = 0; iter < MAX_ITER; ++iter) {
#pragma omp parallel for num_threads(num_threads)
      for (int i = 0; i < size; ++i) {
         double sum = matrix[i][size];
         for (int j = 0; j < size; ++j) {
            if (i != j) sum -= matrix[i][j] * x[j];
         }
         x[i] = sum / matrix[i][i];
      }
      double error = 0.0;
#pragma omp parallel for reduction(+ : error) num_threads(num_threads)
      for (int i = 0; i < size; ++i) error += std::abs(x[i] - prev_x[i]);
      if (error < TOLERANCE) return true;
      prev_x = x;
   }
   return false;
}

void save_results_to_csv(
    const std::string &filename,
    const std::vector<std::tuple<int, int, double, double>> &results) {
   std::ofstream file(filename);
   file << "Size,Threads,SerialTime,ParallelTime\n";
   for (const auto &[size, threads, serial_time, parallel_time] : results) {
      file << size << "," << threads << "," << serial_time << ","
           << parallel_time << "\n";
   }
}

int main() {
   std::vector<int> sizes = {5, 10, 100, 500, 1000, 5000, 10000};
   std::vector<int> thread_counts = {1, 2, 4, 8};
   std::vector<std::tuple<int, int, double, double>> results;

   for (int size : sizes) {
      auto matrix = generate_random_matrix(size);
      for (int threads : thread_counts) {
         std::vector<double> x_serial(size, 0.0), x_parallel(size, 0.0);

         double serial_time =
             measure_time([&]() { gauss_seidel_serial(matrix, x_serial); });
         double parallel_time = measure_time(
             [&]() { gauss_seidel_parallel(matrix, x_parallel, threads); });

         results.emplace_back(size, threads, serial_time, parallel_time);
         std::cout << "Size: " << size << ", Threads: " << threads
                   << ", Serial Time: " << serial_time
                   << ", Parallel Time: " << parallel_time << "\n";
      }
   }

   save_results_to_csv("gauss_seidel_results.csv", results);
   return 0;
}
