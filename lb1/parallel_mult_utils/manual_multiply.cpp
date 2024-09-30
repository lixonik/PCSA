#include <omp.h>
#include "../matrix_utils/matrix.cpp"

#define NUM_THREADS 6

void manual_multiply(const Matrix& firstMatrix, const Matrix& secondMatrix,
                     Matrix& resultMatrix) {
   int size = resultMatrix.getSize();

#pragma omp parallel sections num_threads(NUM_THREADS)
   {
#pragma omp section
      for (int i = 0; i < size / 6; i++) {
         for (int j = 0; j < size; j++) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < size; k++) {
               resultMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
         }
      }

#pragma omp section
      for (int i = size / 6; i < 2 * size / 6; i++) {
         for (int j = 0; j < size; j++) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < size; k++) {
               resultMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
         }
      }

#pragma omp section
      for (int i = 2 * size / 6; i < 3 * size / 6; i++) {
         for (int j = 0; j < size; j++) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < size; k++) {
               resultMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
         }
      }

#pragma omp section
      for (int i = 3 * size / 6; i < 4 * size / 6; i++) {
         for (int j = 0; j < size; j++) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < size; k++) {
               resultMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
         }
      }

#pragma omp section
      for (int i = 4 * size / 6; i < 5 * size / 6; i++) {
         for (int j = 0; j < size; j++) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < size; k++) {
               resultMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
         }
      }

#pragma omp section
      for (int i = 5 * size / 6; i < size; i++) {
         for (int j = 0; j < size; j++) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < size; k++) {
               resultMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
         }
      }
   }
}
