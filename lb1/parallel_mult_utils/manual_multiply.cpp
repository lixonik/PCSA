#include <omp.h>
#include "../matrix_utils/matrix.cpp"

void manual_multiply(const Matrix& firstMatrix, const Matrix& secondMatrix,
                     Matrix& resultMatrix, int num_threads) {
   int size = resultMatrix.getSize();
   int chunk_size = size / num_threads;
   int remainder = size % num_threads;

#pragma omp parallel num_threads(num_threads)
   {
      int thread_id = omp_get_thread_num();
      int start_row = thread_id * chunk_size;
      int end_row = (thread_id + 1) * chunk_size;

      if (thread_id < remainder) {
         start_row += thread_id;
         end_row += thread_id + 1;
      } else {
         start_row += remainder;
         end_row += remainder;
      }

      for (int i = start_row; i < end_row; ++i) {
         for (int j = 0; j < size; ++j) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < size; ++k) {
               resultMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
         }
      }
   }
}
