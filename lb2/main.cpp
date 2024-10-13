#include <mpi.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

void sequentialSelectionSort(std::vector<int>& arr) {
   int n = arr.size();
   for (int i = 0; i < n - 1; i++) {
      int minIndex = i;
      for (int j = i + 1; j < n; j++) {
         if (arr[j] < arr[minIndex]) minIndex = j;
      }
      std::swap(arr[i], arr[minIndex]);
   }
}

void parallelSelectionSort(std::vector<int>& arr, int rank, int size) {
   int n = arr.size();
   int local_size = n / size;
   std::vector<int> local_data(local_size);

   MPI_Scatter(arr.data(), local_size, MPI_INT, local_data.data(), local_size,
               MPI_INT, 0, MPI_COMM_WORLD);

   sequentialSelectionSort(local_data);

   MPI_Gather(local_data.data(), local_size, MPI_INT, arr.data(), local_size,
              MPI_INT, 0, MPI_COMM_WORLD);
}

template <typename Function>
double measure_time(Function fn) {
   auto start = std::chrono::high_resolution_clock::now();
   fn();
   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> duration = end - start;
   return duration.count();
}

std::vector<int> generate_random_data(int size) {
   std::vector<int> data(size);
   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_int_distribution<> dis(1, 10000);
   for (int i = 0; i < size; i++) {
      data[i] = dis(gen);
   }
   return data;
}

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv);

   int rank, size;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   std::vector<int> data_sizes = {10, 100, 500, 1000, 5000};

   if (rank == 0) {
      std::ofstream outfile("times.csv");
      outfile << "size,sequential_time,parallel_time\n";

      for (int n : data_sizes) {
         std::vector<int> data = generate_random_data(n);

         std::vector<int> parallel_data = data;

         double sequential_time =
             measure_time([&]() { sequentialSelectionSort(data); });

         double parallel_time = measure_time(
             [&]() { parallelSelectionSort(parallel_data, rank, size); });

         outfile << n << "," << sequential_time << "," << parallel_time << "\n";
      }

      outfile.close();
   }

   MPI_Finalize();
   return 0;
}
