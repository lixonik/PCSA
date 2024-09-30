#include <omp.h>
#include "../matrix_utils/matrix.cpp"

void for_multiply(const Matrix& firstMatrix, const Matrix& secondMatrix,
                  Matrix& resultMatrix) {

   int size = resultMatrix.getSize();

#pragma omp parallel for num_threads(NUM_THREADS)
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
         resultMatrix[i][j] = 0;
         for (int k = 0; k < size; k++) {
            resultMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
         }
      }
   }
}
