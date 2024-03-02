#include "matrix.hpp"


int main() {

    Matrix<int, 3, 3> matrix;

    std::cin >> matrix;

    std::cout << matrix.determinant();

}