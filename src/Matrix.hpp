#ifndef Matrix_hpp
#define Matrix_hpp

#include <exception>
#include <stdexcept>
#include <iostream>
#include <vector>

struct Matrix {
    std::vector< std::vector<float> > storage;
    Matrix(int rows, int cols);
    Matrix operator*(const Matrix& rhs);
    Matrix operator*(const float& floatNumber);
    Matrix operator/(const float floatNumber);
    int getRowSize();
    int getColSize();
    void toString();
};
#endif /* Matrix_hpp */
