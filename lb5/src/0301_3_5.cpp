#include <omp.h>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void log_step(const std::string &message) { std::cout << message << std::endl; }

template <typename Function>
double measure_time(Function fn) {
   auto start = std::chrono::high_resolution_clock::now();
   fn();
   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> duration = end - start;
   return duration.count();
}

void cholesky_decomposition_sequential(
    const std::vector<std::vector<double>> &A,
    std::vector<std::vector<double>> &L) {
   int n = A.size();
   for (int i = 0; i < n; ++i) {
      for (int j = 0; j <= i; ++j) {
         double sum = 0.0;
         for (int k = 0; k < j; ++k) sum += L[i][k] * L[j][k];

         if (i == j)
            L[i][j] = std::sqrt(A[i][i] - sum);
         else
            L[i][j] = (A[i][j] - sum) / L[j][j];
      }
   }
}

void cholesky_decomposition_parallel(const std::vector<std::vector<double>> &A,
                                     std::vector<std::vector<double>> &L) {
   int n = A.size();
#pragma omp parallel for
   for (int i = 0; i < n; ++i) {
      for (int j = 0; j <= i; ++j) {
         double sum = 0.0;
         for (int k = 0; k < j; ++k) sum += L[i][k] * L[j][k];

         if (i == j)
            L[i][j] = std::sqrt(A[i][i] - sum);
         else
            L[i][j] = (A[i][j] - sum) / L[j][j];
      }
   }
}

void forward_substitution(const std::vector<std::vector<double>> &L,
                          const std::vector<double> &b,
                          std::vector<double> &y) {
   int n = L.size();
   for (int i = 0; i < n; ++i) {
      double sum = 0.0;
      for (int j = 0; j < i; ++j) sum += L[i][j] * y[j];
      y[i] = (b[i] - sum) / L[i][i];
   }
}

void backward_substitution(const std::vector<std::vector<double>> &L,
                           const std::vector<double> &y,
                           std::vector<double> &x) {
   int n = L.size();
   for (int i = n - 1; i >= 0; --i) {
      double sum = 0.0;
      for (int j = i + 1; j < n; ++j) sum += L[j][i] * x[j];
      x[i] = (y[i] - sum) / L[i][i];
   }
}

std::vector<std::vector<double>> generate_spd_matrix(int n) {
   std::vector<std::vector<double>> A(n, std::vector<double>(n, 0.0));
   for (int i = 0; i < n; ++i) {
      for (int j = 0; j <= i; ++j) {
         double value = (rand() % 100) + 1;
         A[i][j] = value;
         A[j][i] = value;
      }
      A[i][i] += n * 10;
   }
   return A;
}

void run_example() {
   log_step("Running example scenario...");

   int n = 4;
   std::vector<std::vector<double>> A = {
       {25, 15, -5, -10}, {15, 18, 0, -6}, {-5, 0, 11, 4}, {-10, -6, 4, 11}};

   log_step("Input matrix:");
   for (const auto &row : A) {
      for (double val : row) std::cout << val << " ";
      std::cout << std::endl;
   }

   std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));
   cholesky_decomposition_sequential(A, L);

   log_step("Lower triangular matrix (L):");
   for (const auto &row : L) {
      for (double val : row) std::cout << val << " ";
      std::cout << std::endl;
   }

   log_step("Example scenario completed.");
}

int main() {
#ifdef DEMO_MODE
   run_example();
#else
   std::vector<int> sizes = {5, 10, 100, 500, 1000};
   std::vector<int> threads = {1, 2, 4, 8};

   std::ofstream csv_file("results.csv");
   csv_file << "Size,Threads,Sequential,Parallel" << std::endl;

   for (int size : sizes) {
      log_step("Processing matrix of size " + std::to_string(size) + "...");
      auto A = generate_spd_matrix(size);
      std::vector<double> b(size, 1.0);

      for (int t : threads) {
         omp_set_num_threads(t);

         log_step("Running with " + std::to_string(t) + " threads...");

         std::vector<std::vector<double>> L_seq(size,
                                                std::vector<double>(size, 0.0));
         double seq_time = measure_time(
             [&]() { cholesky_decomposition_sequential(A, L_seq); });

         std::vector<std::vector<double>> L_par(size,
                                                std::vector<double>(size, 0.0));
         double par_time =
             measure_time([&]() { cholesky_decomposition_parallel(A, L_par); });

         csv_file << size << "," << t << "," << seq_time << "," << par_time
                  << std::endl;
         std::cout << "Size: " << size << ", Threads: " << t
                   << ", Sequential: " << seq_time
                   << "s, Parallel: " << par_time << "s\n";
      }
   }

   csv_file.close();
   log_step("All computations completed. Results saved to results.csv.");
#endif
   return 0;
}