#include <iostream>
#include <cstdlib>

#include "matrix.cpp"

int main(){
    std::printf("Matrix lib test file\n");

    // Create matrix
    Matrix A = Matrix(10, 5, 21.37);
    std::printf("A = [init]\n");
    A.print();
    std::printf("\n");

    A.random();

    std::printf("A = [random]\n");
    A.print();
    std::printf("\n");

    Matrix B = Matrix(10, 5, 73.21);
    std::printf("B = [init]\n");
    B.print();
    std::printf("\n");

    B.random();

    std::printf("B = [random]\n");
    B.print();
    std::printf("\n");

    Matrix C = Matrix(10, 5, 73.21);
    C.random();

    std::printf("C = [random]\n");
    C.print();
    std::printf("\n");

    // Add two together
    Matrix D = A.add(B);

    std::printf("D = [A+B]\n");
    D.print();
    std::printf("\n");

    D.destroy();
    D = A.sub(C);

    std::printf("D = [A-C]\n");
    D.print();
    std::printf("\n");
}