#include <mpi.h>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

template <typename Function>
double measure_time(Function fn) {
   auto start = std::chrono::high_resolution_clock::now();
   fn();
   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> duration = end - start;
   return duration.count();
}

void print_matrix(const std::vector<std::vector<double>>& A) {
   int n = A.size();
   for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
         std::cout << A[i][j] << " ";
      }
      std::cout << std::endl;
   }
}

void print_solution(const std::vector<double>& b) {
   for (const double& value : b) {
      std::cout << value << " ";
   }
   std::cout << std::endl;
}

void gauss_jordan_serial(std::vector<std::vector<double>>& A,
                         std::vector<double>& b) {
   int n = A.size();

   for (int i = 0; i < n; ++i) {
      double pivot = A[i][i];
      for (int j = 0; j < n; ++j) {
         A[i][j] /= pivot;
      }
      b[i] /= pivot;

      std::cout << std::endl
                << "Step " << i + 1 << " (Serial) - Matrix A:" << std::endl;
      print_matrix(A);
      std::cout << std::endl
                << "Step " << i + 1 << " (Serial) - Solution b:" << std::endl;
      print_solution(b);

      for (int j = 0; j < n; ++j) {
         if (i != j) {
            double factor = A[j][i];
            for (int k = 0; k < n; ++k) {
               A[j][k] -= A[i][k] * factor;
            }
            b[j] -= b[i] * factor;
         }
      }

      std::cout << "Step " << i + 1
                << " (Serial) - Updated Matrix A:" << std::endl;
      print_matrix(A);
      std::cout << "Step " << i + 1
                << " (Serial) - Updated Solution b:" << std::endl;
      print_solution(b);
   }
}

void gauss_jordan_parallel(std::vector<std::vector<double>>& A,
                           std::vector<double>& b, int rank, int size) {
   int n = A.size();

   for (int i = 0; i < n; ++i) {
      if (rank == i % size) {
         double pivot = A[i][i];
         for (int j = 0; j < n; ++j) {
            A[i][j] /= pivot;
         }
         b[i] /= pivot;

         std::cout << "Step " << i + 1 << " (Parallel) - Rank " << rank
                   << " - Matrix A:" << std::endl;
         print_matrix(A);
         std::cout << "Step " << i + 1 << " (Parallel) - Rank " << rank
                   << " - Solution b:" << std::endl;
         print_solution(b);

         for (int j = 0; j < n; ++j) {
            if (i != j) {
               double factor = A[j][i];
               for (int k = 0; k < n; ++k) {
                  A[j][k] -= A[i][k] * factor;
               }
               b[j] -= b[i] * factor;
            }
         }

         std::cout << std::endl
                   << "Step " << i + 1 << " (Parallel) - Rank " << rank
                   << " - Updated Matrix A:" << std::endl;
         print_matrix(A);
         std::cout << std::endl
                   << "Step " << i + 1 << " (Parallel) - Rank " << rank
                   << " - Updated Solution b:" << std::endl;
         print_solution(b);
      }
      MPI_Barrier(MPI_COMM_WORLD);
   }
}

void generate_random_system(int n, std::vector<std::vector<double>>& A,
                            std::vector<double>& b) {
   A.resize(n, std::vector<double>(n));
   b.resize(n);

   for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
         A[i][j] = rand() % 100 + 1;
      }
      b[i] = rand() % 100 + 1;
   }
}

void run_test(int n, int num_threads) {
   std::vector<std::vector<double>> A;
   std::vector<double> b;

   generate_random_system(n, A, b);

   int rank, size;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   double time_taken = measure_time([&]() {
      if (size == 1) {
         gauss_jordan_serial(A, b);
      } else {
         gauss_jordan_parallel(A, b, rank, size);
      }
   });

   std::cout << "Time for n=" << n << " and threads=" << num_threads << ": "
             << time_taken << " seconds" << std::endl;
}

void write_to_csv(const std::string& filename,
                  const std::vector<std::vector<double>>& data) {
   std::ofstream file(filename);

   for (const auto& row : data) {
      for (size_t i = 0; i < row.size(); ++i) {
         file << row[i];
         if (i < row.size() - 1) file << ",";
      }
      file << "\n";
   }
}

void demo() {
   int n = 5;
   int num_threads = 2;

   std::cout << "Demonstrating Gauss-Jordan with n=" << n << " and "
             << num_threads << " threads." << std::endl;

   std::vector<std::vector<double>> A;
   std::vector<double> b;

   generate_random_system(n, A, b);

   int rank, size;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   std::cout << "Running serial Gauss-Jordan..." << std::endl;
   gauss_jordan_serial(A, b);
   std::cout << "Running parallel Gauss-Jordan..." << std::endl;
   gauss_jordan_parallel(A, b, rank, size);

   std::cout << "Demo complete." << std::endl;
}

int main(int argc, char* argv[]) {
   MPI_Init(&argc, &argv);

   bool is_demo = false;
   for (int i = 1; i < argc; ++i) {
      if (strcmp(argv[i], "demo") == 0) {
         is_demo = true;
         break;
      }
   }

   if (is_demo) {
      demo();
   } else {
      std::vector<int> sizes = {5, 10, 100, 500, 1000};
      std::vector<int> threads = {1, 2, 4, 8};
      std::vector<std::vector<double>> results;

      for (int n : sizes) {
         for (int num_threads : threads) {
            double time_taken =
                measure_time([&]() { run_test(n, num_threads); });
            results.push_back({(double)n, (double)num_threads, time_taken});
         }
      }

      write_to_csv("results.csv", results);
   }

   MPI_Finalize();
   return 0;
}
