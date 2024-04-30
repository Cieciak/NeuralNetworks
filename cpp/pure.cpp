#include <cstdlib>
#include <iostream>


int power(int base, int exponent){
    if (exponent == 0){return 1;}
    else {return power(base, exponent - 1) * base;}
}

float ReLU(float x){
    if (x > 0){return x;}
    else {return 0.0;}
}

float ReLU_prime(float x){
    if (x > 0){return 1.0;}
    else {return 0.0;}
}

class ShapeException: std::exception {
    private:
        std::string msg;
    public:
        ShapeException (std::string m){msg = m;}
        char* what(){
            return msg.data();
        }
};

class Matrix {
    public:
        int rows;
        int cols;
        float* data;

        Matrix fromTable(int r, int c, float* data){
            Matrix M = Matrix(r, c);

            for (int j = 0; j < M.cols; j++){
                for (int i = 0; i < M.rows; i++){
                    M.setElement(
                        i,
                        j,
                        data[i * M.cols + j]
                    );
                }
            }

            return M;
        }

        Matrix(){
            rows = 0;
            cols = 0;
        }

        Matrix(int r, int c, float init) {
            rows = r;
            cols = c;

            data = (float*) malloc(sizeof(float) * rows * cols);

            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
                    data[i * cols + j] = init;
                }
            }
        }

        Matrix(int r, int c) {
            rows = r;
            cols = c;

            data = (float*) malloc(sizeof(float) * rows * cols);

            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
                    data[i * cols + j] = 0.0;
                }
            }
        }

        void diagonal(){
            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
                    this->setElement(i,j,i+j);
                }
            }
            
        }

        float getElement(int row, int col){
            return data[row * cols + col];
        }

        void setElement(int row, int col, float value){
            data[row * cols + col] = value;
        }

        void print(){
            for (int i = 0; i < rows; i++){
                std::cout << "|";
                for (int j = 0; j < cols; j++){
                    std::cout << " " << data[i * cols + j] << " ";
                }
                std::cout << "|\n";
            }
        }

        Matrix add(Matrix other){
            Matrix M = Matrix(rows, cols, 0.0);
            if (other.cols == 1){
                for (int i = 0; i < rows; i++){
                    for (int j = 0; j < cols; j++){
                        M.setElement(
                            i,
                            j,
                            this->getElement(i, j) + other.getElement(i, 0)
                        );
                    }
                }
            } else {
                for (int i = 0; i < rows; i++){
                    for (int j = 0; j < cols; j++){
                        M.setElement(
                            i,
                            j,
                            this->getElement(i, j) + other.getElement(i,j)
                        );
                    }
                }
            }
            return M;  
        }

        Matrix sub(Matrix other){
            Matrix M = Matrix(rows, cols, 0.0);
            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
                    M.setElement(
                        i,
                        j,
                        this->getElement(i, j) + other.getElement(i,j)
                    );
                }
            }
            return M;
        }

        Matrix scale(float scalar){
            Matrix M = Matrix(rows, cols, 0.0);
            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
                    M.setElement(
                        i,
                        j,
                        this->getElement(i, j) * scalar
                    );
                }
            }
            return M;
        }

        Matrix matmul(Matrix other){
            int k = rows;
            int m = other.cols;

            int l = other.rows;

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
            Matrix M = Matrix(cols, rows, 0.0);
            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
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
            Matrix M = Matrix(rows, cols);
            for (int i = 0; i < rows; i++){
                for (int j = 0; j < rows; j){
                    M.setElement(
                        i,
                        j,
                        func(this->getElement(i, j))
                    );
                }
            }
            return M;
        }

        Matrix sum(int axis){
            if (axis == 0){
                Matrix M = Matrix(1, cols);
                for (int j = 0; j < cols; j++){
                    float element = 0.0;
                    for (int i = 0; i < rows; i++){
                        element += this->getElement(i, j);
                    } 
                    M.setElement(0, j, element);
                }
                return M;
            } else {
                Matrix M = Matrix(rows, 1);
                for (int i = 0; i < rows; i++){
                    float element = 0.0;
                    for (int j = 0; j < rows; j++){
                        element += this->getElement(i, j);
                    } 
                    M.setElement(i, 0, element);
                }
                return M;
            }
        }

        Matrix hadamard(Matrix other){
            Matrix M = Matrix(other.rows, other.cols);
            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
                    M.setElement(
                        i,
                        j,
                        this->getElement(i,j) * other.getElement(i,j)
                    );
                }
            }
            return M;
        }

        Matrix concat(Matrix other){
            Matrix M = Matrix(rows, cols + other.cols);
            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
                    M.setElement(
                        i,
                        j,
                        this->getElement(i, j)
                    );
                }
            }

            for (int i = 0; i < rows; i++){
                for (int j = 0; j < other.cols; j++){
                    M.setElement(
                        i, j + cols, other.getElement(i, j)
                    );
                }
            }
            return M;
        }
};

struct ForwardPropagationResult {
    Matrix* activated;
    Matrix* unactivated;
};

class Network{
    public:
        int* shape;
        int layers;

        Matrix* weights;
        Matrix* biases;
        float alpha;

        Network(int layers, int* shape){
            weights = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            biases  = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));

            for (int index = 0; index < layers - 1; index++){
                weights[index] = Matrix(shape[index + 1], shape[index]);
                biases[index] = Matrix(shape[index + 1], 1);
            }
        }

        ForwardPropagationResult feed(Matrix I){
            Matrix activated[layers];
            Matrix unactiavted[layers];

            activated[0] = I;
            unactiavted[0] = I;

            Matrix A = I;
            for (int index = 0; index < layers - 1; index++){
                Matrix U = (weights[index].matmul(A)).add(biases[index]);
                A = U.map(&ReLU);

                activated[index + 1] = A;
                unactiavted[index + 1] = U; 
            }

            ForwardPropagationResult R;
            R.activated = activated;
            R.unactivated = unactiavted;
            return R;
        }

        Matrix forward(Matrix I){
            ForwardPropagationResult R = this->feed(I);

            return R.activated[layers - 1];
        }

        void back(Matrix I, Matrix O){
            ForwardPropagationResult R = this->feed(I);

            Matrix dZ = R.activated[layers - 1].sub(O);
            for (int index = layers - 1; index > 0; index--){
                int m = R.activated[index].cols;

                Matrix dW = dZ.matmul(R.activated[index - 1].T()).scale(1.0/float(m));
                Matrix dB = dZ.sum(1).scale(1.0/float(m));

                dZ = weights[index - 1].T().matmul(dZ).hadamard(R.unactivated[index - 1].map(&ReLU_prime));

                weights[index - 1] = weights[index - 1].sub(dW.scale(alpha));
                biases[index - 1] = biases[index - 1].sub(dB.scale(alpha));
            }
        }
};

Matrix convert(int number, int size){
    Matrix M = Matrix(size, 1);
    for (int index = 0; index < size; index++){
        float bit = number % 2;
        number    = number / 2;
        M.setElement(
            index,
            0,
            bit
        );
    }

    return M;
}

Matrix oneHot(int place, int size){
    Matrix M = Matrix(size, 1);
    for (int index = 0; index < size; index++){
        if (index == place){
            M.setElement(
                index,
                0,
                1.0
            );
        } else {
            M.setElement(
                index,
                0,
                0.0
            );
        }
    }
    return M;
}

struct DataB {
    Matrix* d;
    Matrix* r;
};

DataB generate(int size){
    Matrix* data = (Matrix*) malloc(sizeof(Matrix) * power(2, size));
    Matrix* resu = (Matrix*) malloc(sizeof(Matrix) * power(2, size));
    for (int index = 0; index < power(2, size); index++){
        data[index] = convert(index, size);
        if (index % 7 == 0){
            resu[index] = oneHot(0, 2);
        } else {
            resu[index] = oneHot(1, 2);
        }
    }

    Matrix* datab = (Matrix*) malloc(sizeof(Matrix) * power(2, size - 3));
    Matrix* resub = (Matrix*) malloc(sizeof(Matrix) * power(2, size - 3));
    for (int i = 0; i < power(2, size - 3); i++){
        Matrix d = Matrix(size, 0);
        Matrix r = Matrix(2, 0);
        for (int j = 0; j < 8; j++){
            d = d.concat(data[i * 8 + j]);
            r = r.concat(resu[i * 8 + j]);
        }
        datab[i] = d;
        resub[i] = r;
    }

    DataB d;
    d.d = datab;
    d.r = resub;

    return d;
}

int main(){
    DataB data = generate(8);

    int layers = 4;
    int* shape = (int*) malloc(4 * layers);
    shape[0] = 8;
    shape[1] = 20;
    shape[2] = 10;
    shape[3] = 2;
    Network NN = Network(layers, shape);
    for (int i = 0; i < 100; i++){
        std::cout << i << std::endl;
        for (int block = 0; block < power(2, 8 - 3); block++){
            NN.back(data.d[block], data.r[block]);
        }
    }

    Matrix r = NN.forward(data.d[0]);

    data.d[0].print();
    std::cout << std::endl;

    data.r[0].print();
    std::cout << std::endl;

    r.print();
    std::cout << std::endl;
}