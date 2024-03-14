#include "matrix.hpp"


int main() {

    Matrix<int, 2, 2> matrix;

    Matrix<int, 2, 2> matrix1;

    std::cin >> matrix;
    std::cin >> matrix1;

    matrix += matrix1;

    std::cout << matrix + matrix1 << '\n';

    std::cout << matrix.determinant() << '\n';

}