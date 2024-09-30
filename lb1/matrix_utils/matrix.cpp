#ifndef MATRIX_CPP
#define MATRIX_CPP

#include <cstdlib>
#include <ctime>
#include <iostream>

class Matrix {
  private:
   int size;
   double **data;

  public:
   Matrix(int n) : size(n) {
      data = new double *[size];
      for (int i = 0; i < size; ++i) {
         data[i] = new double[size];
      }
   }

   ~Matrix() {
      for (int i = 0; i < size; ++i) {
         delete[] data[i];
      }
      delete[] data;
   }

   void initialize() {
      for (int i = 0; i < size; ++i) {
         for (int j = 0; j < size; ++j) {
            data[i][j] = rand() % 10;
         }
      }
   }

   void print() const {
      for (int i = 0; i < size; ++i) {
         for (int j = 0; j < size; ++j) {
            std::cout << data[i][j] << " ";
         }
         std::cout << std::endl;
      }
   }

   // indexed access
   double *operator[](int index) const { return data[index]; }

   bool isEqual(const Matrix &other) const {
      if (size != other.size) return false;
      for (int i = 0; i < size; ++i) {
         for (int j = 0; j < size; ++j) {
            if (data[i][j] != other[i][j]) {
               return false;
            }
         }
      }
      return true;
   }

   int getSize() const { return size; }
};

#endif
