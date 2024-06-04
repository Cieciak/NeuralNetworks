#include <cstdlib>
#include <iostream>

float randf(){ return (float) rand() / RAND_MAX; }

class Matrix {
    public:
        int rows;
        int cols;

        float* data;

        Matrix(){}

        Matrix(int rows, int cols, float init){
            this->rows = rows;
            this->cols = cols;

            this->data = (float* ) malloc( sizeof(float) * this->rows * this->cols );

            for (int index = 0; index < this->rows * this->cols; index++){
                this->data[index] = init;
            }
        }

        void resize(int rows, int cols, float init){
            if (this->data != nullptr){this->destroy();}

            this->rows = rows;
            this->cols = cols;

            this->data = (float* ) malloc( sizeof(float) * this->rows * this->cols );
            for (int index = 0; index < this->rows * this->cols; index++){
                this->data[index] = init;
            }
        }

        void print(){
            for (int row = 0; row < this->rows; row++){
                std::printf("|");
                for (int column = 0; column < this->cols; column++){
                    std::printf(" %8.5f ", this->getElement(row, column));
                }
                std::printf("|\n");
            }
        }

        float getElement(int row, int col){
            if (row < this->rows && col < this->cols){
                int index = row * this->cols + col;
                return this->data[index];
            } else {
                std::printf("getElement\n");
                std::printf("Matrix shape: (%i x %i)\n", this->rows, this->cols);
                std::printf("Tried to acces: (%i, %i)\n", row, col);

                throw std::exception();
            }
        }

        void setElement(int row, int col, float val) {
            if (row < 0 || row >= this->rows || col < 0 || col >= this->cols) {
            std::printf("setElement\n");
            std::printf("Matrix shape: (%i x %i)\n", this->rows, this->cols);
            std::printf("Tried to access: (%i x %i)\n", row, col);
            throw std::out_of_range("Index out of bounds");
            }
            int index = row * this->cols + col;
            this->data[index] = val;
        }


        void random(){
            for (int row = 0; row < this->rows; row++){
                for (int col = 0; col < this->cols; col++){
                    this->setElement(
                        row,
                        col,
                        randf()
                    );
                }
            }
        }

        // Add two matrixes and return the result
        Matrix add(Matrix other){
            Matrix M = Matrix(this->rows, this->cols, 0.0);
            if (other.cols == 1){
                for (int i = 0; i < M.rows; i++){
                    for (int j = 0; j < M.cols; j++){
                        M.setElement(
                            i,
                            j,
                            this->getElement(i, j) + other.getElement(i, 0)
                        );
                    }
                }
            } else {
                for (int i = 0; i < M.rows; i++){
                    for (int j = 0; j < M.cols; j++){
                        M.setElement(
                            i,
                            j,
                            this->getElement(i, j) + other.getElement(i, j)
                        );
                    }
                }
            }
            return M;
        }

        // Add two matrixes in-place
        void addi(Matrix other){
            if (other.cols == 1){
                for (int i = 0; i < this->rows; i++){
                    for (int j = 0; j < this->cols; j++){
                        this->setElement(
                            i,
                            j,
                            this->getElement(i, j) + other.getElement(i, 0)
                        );
                    }
                }
            } else {
                for (int i = 0; i < this->rows; i++){
                    for (int j = 0; j < this->cols; j++){
                        this->setElement(
                            i,
                            j,
                            this->getElement(i, j) + other.getElement(i, j)
                        );
                    }
                }
            }
        }

        // Subtract two matrixes and return the result
        Matrix sub(Matrix other){
            Matrix M = Matrix(this->rows, this->cols, 0.0);

            if (other.cols == 1){
                for (int i = 0; i < M.rows; i++){
                    for (int j = 0; j < M.cols; j++){
                        M.setElement(
                            i,
                            j,
                            this->getElement(i, j) - other.getElement(i, 0)
                        );
                    }
                }
            } else {
                for (int i = 0; i < M.rows; i++){
                    for (int j = 0; j < M.cols; j++){
                        M.setElement(
                            i,
                            j,
                            this->getElement(i, j) - other.getElement(i, j)
                        );
                    }
                }
            }

            return M;
        }

        // Subtract two matrixes in-place
        void subi(Matrix other){
            if (other.cols == 1){
                for (int i = 0; i < this->rows; i++){
                    for (int j = 0; j < this->cols; j++){
                        this->setElement(
                            i,
                            j,
                            this->getElement(i, j) + other.getElement(i, 0)
                        );
                    }
                }
            } else {
                for (int i = 0; i < this->rows; i++){
                    for (int j = 0; j < this->cols; j++){
                        this->setElement(
                            i,
                            j,
                            this->getElement(i, j) + other.getElement(i, j)
                        );
                    }
                }
            }
        }

        // Scale matrix and return result
        Matrix scale(float scalar){
            Matrix M = Matrix(this->rows, this->cols, 0.0);
            for (int i = 0; i < this->rows; i++){
                for (int j = 0; j < this->cols; j++){
                    M.setElement(
                        i,
                        j,
                        this->getElement(i, j) * scalar
                    );
                }
            }
            return M;
        }

        // Scale in-place
        void scalei(float scalar){
            for (int i = 0; i < this->rows; i++){
                for (int j = 0; j < this->cols; j++){
                    this->setElement(
                        i,
                        j,
                        this->getElement(i, j) * scalar
                    );
                }
            }
        }

        Matrix matmul(Matrix other){

            int k = this->rows;
            int l = this->cols;
            int m = other.cols;

            Matrix M = Matrix(k, m, 0.0);
            for (int i = 0; i < M.rows; i++){
                for (int j = 0; j < M.cols; j++){
                    float element = 0.0;
                    for (int x = 0; x < l; x++){
                        element += this->getElement(i, x) * other.getElement(x, j);
                    }
                    M.setElement(
                        i,
                        j,
                        element
                    );
                }
            }
            return M;
        }

        Matrix T(){
            Matrix M = Matrix(this->cols, this->rows, 0.0);
            for (int i = 0; i < this->rows; i++){
                for (int j = 0; j < this->cols; j++){
                    M.setElement(
                        j,
                        i,
                        this->getElement(i, j)
                    );
                }
            }
            return M;
        }

        Matrix map(float (*func)(float)){
            Matrix M = Matrix(this->rows, this->cols, 0.0);
            for (int i = 0; i < this->rows; i++){
                for (int j = 0; j < this->cols; j++){
                    M.setElement(
                        i,
                        j,
                        func(this->getElement(i, j))
                    );
                }
            }
            return M;
        }

        void mapi(float (*func)(float)){
            for (int i = 0; i < this->rows; i++){
                for (int j = 0; j < this->cols; j++){
                    this->setElement(
                        i,
                        j,
                        func(this->getElement(i, j))
                    );
                }
            }
        }

        Matrix sum(int axis){
            if (axis == 0){
                Matrix M = Matrix(1, this->cols, 0.0);
                for (int j = 0; j < this->cols; j++){
                    float element = 0.0;
                    for (int i = 0; i < this->rows; i++){
                        element += this->getElement(i, j);
                    } 
                    M.setElement(0, j, element);
                }
                return M;
            } else {
                Matrix M = Matrix(this->rows, 1, 0.0);
                for (int i = 0; i < this->rows; i++){
                    float element = 0.0;
                    for (int j = 0; j < this->cols; j++){
                        element += this->getElement(i, j);
                    } 
                    M.setElement(i, 0, element);
                }
                return M;
            }
        }

        Matrix hadamard(Matrix other){
            Matrix M = Matrix(this->rows, this->cols, 0.0);
            for (int i = 0; i < this->rows; i++){
                for (int j = 0; j < this->cols; j++){
                    M.setElement(
                        i,
                        j,
                        this->getElement(i, j) * other.getElement(i, j)
                    );
                }
            }
            return M;
        }

        void hadamardi(Matrix other){
            for (int i = 0; i < this->rows; i++){
                for (int j = 0; j < this->cols; j++){
                    this->setElement(
                        i,
                        j,
                        this->getElement(i, j) * other.getElement(i, j)
                    );
                }
            }
        }

        Matrix concat(Matrix other){
            Matrix M = Matrix(this->rows, this->cols + other.cols, 0.0);
            for (int i = 0; i < this->rows; i++){
                for (int j = 0; j < this->cols; j++){
                    M.setElement(
                        i,
                        j,
                        this->getElement(i, j)
                    );
                }
            }

            for (int i = 0; i < this->rows; i++){
                for (int j = 0; j < other.cols; j++){
                    M.setElement(
                        i,
                        j + cols,
                        other.getElement(i, j)
                    );
                }
            }
            return M;
        }

        void destroy(){free(this->data);}
};
