#include <iostream>
#include <omp.h>
#include "./matrix_utils/matrix.cpp"
#include "./parallel_mult_utils/for_multiply.cpp"
#include "./parallel_mult_utils/manual_multiply.cpp"
#include "./parallel_mult_utils/serial_multiply.cpp"

#define SIZE 4

int main() {
    srand(time(0));

    Matrix m1(SIZE), m2(SIZE), result_serial(SIZE), result_parallel_for(SIZE), result_parallel_manual(SIZE);
    m1.initialize();
    m2.initialize();

    std::cout << "Matrix 1:" << std::endl;
    m1.print();
    std::cout << "Matrix 2:" << std::endl;
    m2.print();

    // parallel serial
    serial_multiply(m1, m2, result_serial);
    std::cout << "\nResult (Serial):" << std::endl;
    result_serial.print();

    // parallel #pragma omp for
    for_multiply(m1, m2, result_parallel_for);
    std::cout << "\nResult (Parallel for):" << std::endl;
    result_parallel_for.print();

    // parallel manal
    manual_multiply(m1, m2, result_parallel_manual);
    std::cout << "\nResult (Parallel manual):" << std::endl;
    result_parallel_manual.print();

    // assert
    if (result_serial.isEqual(result_parallel_for) && result_serial.isEqual(result_parallel_manual)) {
        std::cout << "\nAll results are correct and match each other!" << std::endl;
    } else {
        std::cout << "\nError: The results do not match!" << std::endl;
    }

    return 0;
}
