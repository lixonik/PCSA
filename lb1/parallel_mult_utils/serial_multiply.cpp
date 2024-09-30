#include "../matrix_utils/matrix.cpp"

void serial_multiply(const Matrix &firstMatrix, const Matrix &secondMatrix,
                     Matrix &resultMatrix) {
   int size = firstMatrix.getSize();

   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
         resultMatrix[i][j] = 0;
         for (int k = 0; k < size; k++) {
            resultMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
         }
      }
   }
}
