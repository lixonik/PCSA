#include <omp.h>
#include <iostream>
#include "./matrix_utils/matrix.cpp"
#include "./parallel_mult_utils/for_multiply.cpp"
#include "./parallel_mult_utils/manual_multiply.cpp"
#include "./parallel_mult_utils/serial_multiply.cpp"

#define SIZE 4

int main() {
   srand(time(0));

   Matrix firstMatrix(SIZE), secondMatrix(SIZE);
   Matrix result_serial(SIZE), result_parallel_for(SIZE),
       result_parallel_manual(SIZE);

   firstMatrix.initialize();
   secondMatrix.initialize();

   std::cout << "Matrix 1:" << std::endl;
   firstMatrix.print();
   std::cout << "Matrix 2:" << std::endl;
   secondMatrix.print();

   // parallel serial
   serial_multiply(firstMatrix, secondMatrix, result_serial);
   std::cout << "\nResult (Serial):" << std::endl;
   result_serial.print();

   // parallel #pragma omp for
   for_multiply(firstMatrix, secondMatrix, result_parallel_for);
   std::cout << "\nResult (Parallel for):" << std::endl;
   result_parallel_for.print();

   // parallel manal
   manual_multiply(firstMatrix, secondMatrix, result_parallel_manual);
   std::cout << "\nResult (Parallel manual):" << std::endl;
   result_parallel_manual.print();

   // assert
   if (result_serial.isEqual(result_parallel_for) &&
       result_serial.isEqual(result_parallel_manual)) {
      std::cout << "\nAll results are correct and match each other!"
                << std::endl;
   } else {
      std::cout << "\nError: The results do not match!" << std::endl;
   }

   return 0;
}
