#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <ctime>

typedef float (*realFunc)(float);

float randf(){return (float) rand() / RAND_MAX;}

void init(){
    srand(time(0));
}

void throw_error(std::string msg) {
    throw std::runtime_error(msg);
}

class Matrix{
    public:
        int rows;
        int cols;

        float* data;

        Matrix(){}

        Matrix(int rows, int cols, float init){
            if (rows == 0 || cols == 0){
                std::ostringstream oss;
                oss << "Cannot create matrix of size: (" << rows << ", " << cols << ")";
                throw_error(oss.str());
            }

            this->rows = rows;
            this->cols = cols;
            this->data = (float*) malloc(sizeof(float) * this->rows * this->cols);

            for (int index = 0; index < this->rows * this->cols; index++){this->data[index] = init;}
        }

        void printf(){
            for (int row = 0; row < this->rows; row++){
                std::printf("|");
                for (int col = 0; col < this->cols; col++){
                    std::printf(" %8.5f ", this->getElement(row, col));
                }
                std::printf("|\n");
            }
        }

        float getElement(int row, int col){
            if (row < 0 || this->rows <= row || col < 0 || this->cols <= col){
                std::ostringstream oss;
                oss << "Cannot access element (" << row << " " << col << ") of matrix (" << this->rows << " " << this->cols << ")";
                throw_error(oss.str());
            }
            int index = row * this->cols + col;
            return this->data[index];
        }

        void setElement(int row, int col, float val){
            if (row < 0 || this->rows <= row || col < 0 || this->cols <= col){
                std::ostringstream oss;
                oss << "Cannot access element (" << row << " " << col << ") of matrix (" << this->rows << " " << this->cols << ")";
                throw_error(oss.str());
            }
            int index = row * this->cols + col;
            this->data[index] = val;
        }

        Matrix operator+(Matrix other){
            if (this->rows != other.rows){
                std::ostringstream oss;
                oss << "Cannot add matrices of size (" << this->rows << " " << this->cols << ") and (" << other.rows << " " << other.cols << ")";
                throw_error(oss.str());
            }

            Matrix M = Matrix(this->rows, this->cols, 0.0);
            // Handle vector addition
            if (other.cols == 1){
                for (int x = 0; x < this->rows; x++){
                    for (int y = 0; y < this->cols; y++){
                        M.setElement(
                            x, y,
                            this->getElement(x, y) + other.getElement(x, 0)
                        );
                    }
                }
            }
            // Normal matrix addition
            else {
                for (int x = 0; x < this->rows; x++){
                    for (int y = 0; y < this->cols; y++){
                        M.setElement(
                            x,y,
                            this->getElement(x, y) + other.getElement(x, y)
                        );
                    }
                }
            }

            return M;
        }

        Matrix operator-(Matrix other){
            if (this->rows != other.rows){
                std::ostringstream oss;
                oss << "Cannot subtract matrices of size (" << this->rows << " " << this->cols << ") and (" << other.rows << " " << other.cols << ")";
                throw_error(oss.str());
            }

            Matrix M = Matrix(this->rows, this->cols, 0.0);
            // Handle vector addition
            if (other.cols == 1){
                for (int x = 0; x < this->rows; x++){
                    for (int y = 0; y < this->cols; y++){
                        M.setElement(
                            x, y,
                            this->getElement(x, y) - other.getElement(x, 0)
                        );
                    }
                }
            }
            // Normal matrix addition
            else {
                for (int x = 0; x < this->rows; x++){
                    for (int y = 0; y < this->cols; y++){
                        M.setElement(
                            x,y,
                            this->getElement(x, y) - other.getElement(x, y)
                        );
                    }
                }
            }

            return M;
        }

        Matrix scale(float scalar){
            Matrix M = Matrix(this->rows, this->cols, 0.0);
            for (int x = 0; x < this->rows; x++){
                for (int y = 0; y < this->cols; y++){
                    M.setElement(
                        x,y,
                        this->getElement(x,y) * scalar
                    );
                }
            }

            return M;
        }

        Matrix transpose(){
            Matrix M = Matrix(this->cols, this->rows, 0.0);
            for (int x = 0; x < this->rows; x++){
                for (int y = 0; y < this->cols; y++){
                    M.setElement(y, x, this->getElement(x, y));
                }
            }

            return M;
        }

        Matrix map(realFunc func){
            Matrix M = Matrix(this->rows, this->cols, 0.0);
            for (int x = 0; x < this->rows; x++){
                for (int y = 0; y < this->cols; y++){
                    M.setElement(x, y, func(this->getElement(x, y)));
                }
            }

            return M;
        }

        Matrix sum(int axis){
            if (axis == 0){
                Matrix M = Matrix(1, this->cols, 0.0);
                for (int y = 0; y < this->cols; y++){
                    float element = 0.0;
                    for (int x = 0; x < this->rows; x++){element += this->getElement(x, y);}
                    M.setElement(0, y, element);
                }
                return M;
            } else {
                Matrix M = Matrix(this->rows, 1, 0.0);
                for (int x = 0; x < this->rows; x++){
                    float element = 0.0;
                    for (int y = 0; y < this->cols; y++){element += this->getElement(x,y);}
                    M.setElement(x, 0, element);
                }
                return M;
            }
        }
};

Matrix randomMatrix(int rows, int cols){
    Matrix M = Matrix(rows, cols, 0.0);
    for (int row = 0; row < rows; row++){
        for (int col = 0; col < cols; col++){
            M.setElement(row, col, randf());
        }
    }
    return M;
}

Matrix matmul(Matrix A, Matrix B){
    int k  = A.rows;
    int l0 = A.cols;
    int l1 = B.rows;
    int m  = B.cols;

    if (l0 != l1){
        std::printf("Cannot multiply (%i, %i) with (%i, %i)", k, l0, l1, m);
        throw "Matmul size err";
    }

    Matrix M = Matrix(k, m, 0.0);
    for (int x = 0; x < M.rows; x++){
        for (int y = 0; y < M.cols; y++){
            // Cross multiply
            float element = 0.0;
            for (int i = 0; i < l0; i++){element += A.getElement(x, i) * B.getElement(i, y);}

            M.setElement(x, y, element);
        }
    }
    return M;
}

Matrix hadamard(Matrix A, Matrix B){
    if (A.rows != B.rows || A.cols != B.cols){
        std::printf("Cannot multiply (%i, %i) with (%i, %i)", A.rows, A.cols, B.rows, B.cols);
        throw "Hadamard size err";        
    }

    Matrix M = Matrix(A.rows, B.cols, 0.0);
    for (int x = 0; x < M.rows; x++){
        for (int y = 0; y < M.cols; y++){
            M.setElement(x, y, A.getElement(x, y) * B.getElement(x, y));
        }
    }

    return M;
}

Matrix concat(Matrix A, Matrix B){
    if (A.rows != B.rows){
        std::printf("Cannot concat");
        throw "Concatination error";           
    }
    Matrix M = Matrix(A.rows, A.cols + B.cols, 0.0);
    for (int x = 0; x < M.rows; x++){
        for (int y = 0; y < A.cols; y++){
            M.setElement(x, y, A.getElement(x, y));    
        }
    }

    for (int x = 0; x < M.rows; x++){
        for (int y = 0; y < B.cols; y++){
            M.setElement(x, y + A.cols, B.getElement(x, y));
        }
    }
    
    return M;
}