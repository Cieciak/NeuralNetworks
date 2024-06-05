#include <cstdlib>

#include "matrix.cpp"

float checkMatAdd(Matrix A, Matrix B, Matrix R){
    float propertyA = 0.0;
    float propertyB = 0.0;
    float propertyR = 0.0;

    for (int x = 0; x < A.rows; x++){
        for (int y = 0; y < A.cols; y++){
            propertyA += A.getElement(x, y);
        }
    }

    for (int x = 0; x < B.rows; x++){
        for (int y = 0; y < B.cols; y++){
            propertyB += B.getElement(x, y);
        }
    }

    for (int x = 0; x < R.rows; x++){
        for (int y = 0; y < R.cols; y++){
            propertyR += R.getElement(x, y);
        }
    }

    return (propertyR-propertyA-propertyB) / (R.rows * R.cols);
}

float checkMatSub(Matrix A, Matrix B, Matrix R){
    float propertyA = 0.0;
    float propertyB = 0.0;
    float propertyR = 0.0;

    for (int x = 0; x < A.rows; x++){
        for (int y = 0; y < A.cols; y++){
            propertyA += A.getElement(x, y);
        }
    }

    for (int x = 0; x < B.rows; x++){
        for (int y = 0; y < B.cols; y++){
            propertyB += B.getElement(x, y);
        }
    }

    for (int x = 0; x < R.rows; x++){
        for (int y = 0; y < R.cols; y++){
            propertyR += R.getElement(x, y);
        }
    }

    return (propertyR-propertyA+propertyB) / (R.rows * R.cols);
}

float checkMatSca(Matrix A, float scalar, Matrix R){
    float propertyA = 0.0;
    float propertyR = 0.0;

    for (int x = 0; x < A.rows; x++){
        for (int y = 0; y < A.cols; y++){
            propertyA += A.getElement(x, y);
        }
    }

    for (int x = 0; x < R.rows; x++){
        for (int y = 0; y < R.cols; y++){
            propertyR += R.getElement(x, y);
        }
    }
    
    return (propertyR - propertyA * scalar) / (R.rows * R.cols);
}

float checkMatTra(Matrix A, Matrix R){
    // A.t + N - R = N
    float propertyD = 0.0;
    float propertyN = 0.0;

    Matrix N = randomMatrix(A.cols, A.rows);
    Matrix T = A.transpose() + N;

    Matrix D = T - R;

    for (int x = 0; x < D.rows; x++){
        for (int y = 0; y < D.cols; y++){
            propertyD += D.getElement(x, y);
        }
    }

    for (int x = 0; x < N.rows; x++){
        for (int y = 0; y < N.cols; y++){
            propertyN += N.getElement(x, y);
        }
    }

    return (propertyD - propertyN) / ((R.rows * R.cols));
}

float checkMatMul(Matrix A, Matrix B){
    // (AB).t = (B.t)(A.t)
    float propertyL = 0.0;
    float propertyR = 0.0;

    Matrix L = matmul(A, B).transpose();
    Matrix R = matmul(B.transpose(), A.transpose());

    for (int x = 0; x < L.rows; x++){
        for (int y = 0; y < L.cols; y++){
            propertyL += L.getElement(x, y);
        }
    }

    for (int x = 0; x < R.rows; x++){
        for (int y = 0; y < R.cols; y++){
            propertyR += R.getElement(x, y);
        }
    }

    return (propertyL - propertyR) / (R.rows * R.cols);
}

float checkMatMap(Matrix A){
    float propertyA = 0.0;
    float propertyS = 0.0;

    realFunc square = [](float x) -> float {return x * x;};

    Matrix S = A.map(square);

    for (int x = 0; x < A.rows; x++){
        for (int y = 0; y < A.cols; y++){
            propertyA += A.getElement(x, y) * A.getElement(x, y);
        }
    }

    for (int x = 0; x < S.rows; x++){
        for (int y = 0; y < S.cols; y++){
            propertyS += S.getElement(x, y);
        }
    }

    return (propertyA - propertyS) / (A.rows * A.cols);
}

float checkMatHad(Matrix A, Matrix B){
    float propertyL = 0.0;
    float propertyR = 0.0;

    Matrix N = randomMatrix(A.rows, A.cols);
    Matrix L = hadamard(A, B + N);
    Matrix R = hadamard(A, B) + hadamard(A, N);

    for (int x = 0; x < L.rows; x++){
        for (int y = 0; y < L.cols; y++){
            propertyL += L.getElement(x, y);
        }
    }

    for (int x = 0; x < R.rows; x++){
        for (int y = 0; y < R.cols; y++){
            propertyR += R.getElement(x, y);
        }
    }

    return (propertyL - propertyR) / (R.rows * R.cols);
}

float checkMatSum(Matrix A){
    float propertyL = 0.0;
    float propertyR = 0.0;

    Matrix L = A.sum(0).sum(1);
    Matrix R = A.sum(1).sum(0);

    for (int x = 0; x < L.rows; x++){
        for (int y = 0; y < L.cols; y++){
            propertyL += L.getElement(x, y);
        }
    }

    for (int x = 0; x < R.rows; x++){
        for (int y = 0; y < R.cols; y++){
            propertyR += R.getElement(x, y);
        }
    }

    return (propertyL - propertyR) / (A.rows * A.cols);
}

float checkMatCon(Matrix A, Matrix B){
    float propertyA = 0.0;
    float propertyB = 0.0;
    float propertyC = 0.0;

    Matrix C = concat(A, B);

    for (int x = 0; x < A.rows; x++){
        for (int y = 0; y < A.cols; y++){
            propertyA += A.getElement(x, y);
        }
    }

    for (int x = 0; x < B.rows; x++){
        for (int y = 0; y < B.cols; y++){
            propertyB += B.getElement(x, y);
        }
    }

    for (int x = 0; x < C.rows; x++){
        for (int y = 0; y < C.cols; y++){
            propertyC += C.getElement(x, y);
        }
    }

    return (propertyC - propertyA - propertyB) / (C.rows * C.cols);
}

int main(){
    init();

    int limit = 100;
    for (int size = 1; size <= limit; size++){
        Matrix A = randomMatrix(size, size);
        Matrix B = randomMatrix(size, size);

        printf("A:(%i, %i), B:(%i, %i)\n", A.rows, A.cols, B.rows, B.cols);
        printf("Error add: %9.6f\n", checkMatAdd(A,B,A+B));
        printf("Error sub: %9.6f\n", checkMatSub(A,B,A-B));

        float s = randf();
        printf("Error sca: %9.6f\n", checkMatSca(A,s, A.scale(s)));
        printf("Error tra: %9.6f\n", checkMatTra(A, A.transpose()));
        printf("Error mul: %9.6f\n", checkMatTra(A, A.transpose()));
        printf("Error map: %9.6f\n", checkMatMap(A));
        printf("Error had: %9.6f\n", checkMatHad(A, B));
        printf("Error sum: %9.6f\n", checkMatSum(A));
        printf("Error con: %9.6f\n", checkMatCon(A, B));
    }

    limit = 25;
    for (int width = 1; width <= limit; width++){
        for (int height = 1; height <= limit; height++){
            Matrix A = randomMatrix(width, height);
            Matrix B = randomMatrix(width, height);

            printf("A:(%i, %i), B:(%i, %i)\n", A.rows, A.cols, B.rows, B.cols);
            printf("Error add: %9.6f\n", checkMatAdd(A,B,A+B));
            printf("Error sub: %9.6f\n", checkMatSub(A,B,A-B));

            float s = randf();
            printf("Error sca: %9.6f\n", checkMatSca(A,s, A.scale(s)));
            printf("Error mul: %9.6f\n", checkMatTra(A, A.transpose()));
            printf("Error map: %9.6f\n", checkMatMap(A));
            printf("Error had: %9.6f\n", checkMatHad(A, B));
            printf("Error sum: %9.6f\n", checkMatSum(A));
            printf("Error con: %9.6f\n", checkMatCon(A, B));
        }
    }

    return 0;
}