#include "Matrix.hpp"

Matrix::Matrix(int rows, int cols) {
  storage.resize(rows);
  for (auto& row: storage) {
    row.resize(cols);
  }
}

Matrix Matrix::operator*(const Matrix& rhs) {
  if (storage[0].size() != rhs.storage.size()) {
    throw std::invalid_argument{ "The number of cols in the first matrix doesn't match the rows in the second one!" };
  }
  
  int resultRows = (int) storage.size();
  int resultCols = (int) rhs.storage[0].size();
  
  Matrix result{ resultRows, resultCols };
  
  for (int row = 0; row < resultRows; row++) {
    for (int col = 0; col < resultCols; col++) {
      for (int i = 0; i < storage[0].size(); i++) {
        result.storage[row][col] += storage[row][i] * rhs.storage[i][col];
      }
    }
  }
  
  return result;
  
}
int Matrix::getColSize() {
  return (int)storage[0].size();
}
int Matrix::getRowSize() {
  return (int)storage.size();
}

Matrix Matrix::operator*(const float &floatNumber) {
  Matrix result{getRowSize(), getColSize()};
    for(int r = 0; r < getRowSize(); r++) {
      for(int c = 0; c < getColSize(); c++) {
        result.storage[r][c] = storage[r][c]*floatNumber;
      }
    }
  return result;
}

Matrix Matrix::operator/(const float floatNumber) {
    Matrix result{getRowSize(), getColSize()};
    for(int r = 0; r < getRowSize(); r++) {
        for(int c = 0; c < getColSize(); c++) {
            result.storage[r][c] = storage[r][c]/floatNumber;
        }
    }
    return result;
}


void Matrix::toString() {
    for(int r = 0; r < getRowSize(); r++) {
        for(int c = 0; c < getColSize(); c++) {
            std::cout << storage[r][c] << " ";
        }
        std::cout << std::endl;
    }
}