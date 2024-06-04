#include <cstdlib>
#include "matrix.cpp"

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

struct ForwardPropagationResult {
    Matrix* A;
    Matrix* U;
};

class Network{
    public:
        int layers;
        int* shape;

        float alpha;

        Matrix* weights;
        Matrix* biases;

        Network(int layers, int* shape, float alpha){
            this->layers = layers;
            this->shape = shape;
            this->alpha = alpha;

            this->weights = (Matrix*) malloc( sizeof(Matrix) * (layers - 1) );
            this->biases  = (Matrix*) malloc( sizeof(Matrix) * (layers - 1) );

            for (int index = 0; index < layers - 1; index++){
                this->weights[index] = Matrix(shape[index + 1], shape[index], 0.0);
                this->biases[index]  = Matrix(shape[index + 1],            1, 0.0);

                this->weights[index].random();
                this->biases[index].random();
            }
        }

        ForwardPropagationResult feed(Matrix I){
            ForwardPropagationResult result;

            result.A = (Matrix*) malloc( sizeof(Matrix) * layers );
            result.U = (Matrix*) malloc( sizeof(Matrix) * layers );

            result.A[0] = I;
            result.U[0] = I;

            Matrix A = I;
            for (int index = 0; index < this->layers - 1; index++){
                Matrix U = (this->weights[index].matmul(A)).add(this->biases[index]);
                
                A = U.map(&ReLU);

                result.A[index + 1] = A;
                result.U[index + 1] = U;
            }
            return result;
        }

        Matrix forward(Matrix I){
            ForwardPropagationResult R = this->feed(I);
            return R.A[this->layers - 1];
        }

        void backward(Matrix I, Matrix O){
            ForwardPropagationResult R = this->feed(I);
            Matrix dZ = R.A[this->layers - 1].sub(O);
            for (int index = layers - 1; index > 0; index--){
                // Get batch size
                int m = R.A[index].cols;

                Matrix dW = dZ.matmul(R.A[index - 1].T()).scale(1.0/float(m));
                Matrix dB = dZ.scale(1.0/float(m));

                dZ = this->weights[index - 1].T().matmul(dZ).hadamard(R.U[index - 1].map(&ReLU_prime));

                this->weights[index - 1] = this->weights[index - 1].sub(dW.scale(this->alpha));
                this->biases[index - 1] = this->biases[index - 1].sub(dB.scale(this->alpha));
            }
        }
};


