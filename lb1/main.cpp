#include <chrono>
#include <fstream>
#include "./matrix_utils/matrix.cpp"
#include "./parallel_mult_utils/for_multiply.cpp"
#include "./parallel_mult_utils/manual_multiply.cpp"
#include "./parallel_mult_utils/serial_multiply.cpp"

template <typename Function>
double measure_time(Function fn) {
   auto start = std::chrono::high_resolution_clock::now();
   fn();
   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> duration = end - start;
   return duration.count();
}

int main() {
   std::ofstream results_file("matrix_multiply.csv");
   results_file << "MatrixSize,Threads,Serial,ParallelFor,ParallelManual\n";

   int sizes[] = {10, 100, 500, 1000};
   int threads[] = {2, 4, 8};

   for (int num_threads : threads) {
      for (int size : sizes) {
         std::cout << "Testing matrix size: " << size << " with " << num_threads
                   << " threads" << std::endl;

         Matrix firstMatrix(size), secondMatrix(size);
         Matrix result_serial(size), result_parallel_for(size),
             result_parallel_manual(size);

         firstMatrix.initialize();
         secondMatrix.initialize();

         // Serial multiplication
         double serial_time = measure_time([&]() {
            serial_multiply(firstMatrix, secondMatrix, result_serial);
         });

         // Parallel with #pragma omp for
         double parallel_for_time = measure_time([&]() {
            for_multiply(firstMatrix, secondMatrix, result_parallel_for,
                         num_threads);
         });

         // Parallel manual multiplication
         double parallel_manual_time = measure_time([&]() {
            manual_multiply(firstMatrix, secondMatrix, result_parallel_manual,
                            num_threads);
         });

         if (result_serial.isEqual(result_parallel_for) &&
             result_serial.isEqual(result_parallel_manual)) {
            std::cout << "Results match for size " << size << " and threads "
                      << num_threads << std::endl;
         } else {
            std::cout << "Error: Results do not match for size " << size
                      << " and threads " << num_threads << std::endl;
         }

         results_file << size << "," << num_threads << "," << serial_time << ","
                      << parallel_for_time << "," << parallel_manual_time
                      << "\n";
      }
   }

   results_file.close();
   return 0;
}