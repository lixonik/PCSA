#include <mpi.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

void sequential_selection_sort(std::vector<int>& arr) {
   int n = arr.size();
   for (int i = 0; i < n - 1; i++) {
      int minIndex = i;
      for (int j = i + 1; j < n; j++) {
         if (arr[j] < arr[minIndex]) minIndex = j;
      }
      std::swap(arr[i], arr[minIndex]);
   }
}

int get_local_minimum(int* arr, int start, int size) {
   int minIdx = start;
   for (int i = start + 1; i < size; i++) {
      if (arr[minIdx] > arr[i]) minIdx = i;
   }
   std::swap(arr[start], arr[minIdx]);
   return arr[start];
}

void parallel_selection_sort(std::vector<int>& arr, int rank, int size,
                             int world_size) {
   int n = arr.size();
   int local_size = n / world_size;

   std::vector<int> local_data(local_size);

   MPI_Scatter(arr.data(), local_size, MPI_INT, local_data.data(), local_size,
               MPI_INT, 0, MPI_COMM_WORLD);

   // Local sorting for each part of the array
   int local_min = get_local_minimum(local_data.data(), 0, local_size);

   int global_min;
   int process_num;

   for (int step = 0; step < n; step++) {
      if (rank == 0) {
         global_min = local_min;
         process_num = 0;
      }

      for (int i = 1; i < world_size; i++) {
         if (rank == 0) {
            int received_min;
            MPI_Recv(&received_min, 1, MPI_INT, i, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            if (received_min < global_min) {
               global_min = received_min;
               process_num = i;
            }
         } else if (rank == i) {
            MPI_Send(&local_min, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
         }
      }

      MPI_Bcast(&process_num, 1, MPI_INT, 0, MPI_COMM_WORLD);

      if (rank == process_num) {
         local_min = get_local_minimum(
             local_data.data(), 1,
             local_size);  // Updating the minimum element for this process
      }

      MPI_Barrier(MPI_COMM_WORLD);
   }

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

template <typename... Args>
std::string format_string(const Args&... args) {
   std::ostringstream oss;
   (oss << ... << args);
   return oss.str();
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

   int rank, world_size;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &world_size);

   std::vector<int> data_sizes = {10, 100, 500, 1000, 5000};

   std::string output_filename =
       format_string("measure111ments_", world_size, ".csv");
   std::string output_path = format_string("./measurements/", output_filename);

   if (rank == 0) {
      std::ofstream outfile(output_path);
      outfile << "size,sequential_time,parallel_time\n";
   }

   for (int n : data_sizes) {
      std::vector<int> data = generate_random_data(n);
      std::vector<int> parallel_data = data;

      // Measure the running time of the sequential algorithm
      double sequential_time =
          measure_time([&]() { sequential_selection_sort(data); });

      // Measure the running time of the parallel algorithm
      double parallel_time = measure_time([&]() {
         parallel_selection_sort(parallel_data, rank, n, world_size);
      });

      if (rank == 0) {
         std::ofstream outfile(output_path, std::ios_base::app);
         outfile << n << "," << sequential_time << "," << parallel_time << "\n";
      }

      MPI_Barrier(MPI_COMM_WORLD);
   }

   MPI_Finalize();
   return 0;
}
