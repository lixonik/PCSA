#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
private:
    int size;
    double **data;

public:
    Matrix(int n);

    ~Matrix();

    void initialize();

    void print() const;

    // indexed access
    double *operator[](int index) const;

    bool isEqual(const Matrix &other) const;

    int getSize() const;
};

#endif
