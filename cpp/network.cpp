#include <cstdlib>

#include "matrix.cpp"

float ReLU(float x){
    if (x > 0){return x;}
    else {return 0.0;}
}

float ReLU_prime(float x){
    if (x > 0){return 1.0;}
    else {return 0.0;}
}

struct ForwardPropagationResult{
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

            this->weights = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));
            this->biases  = (Matrix*) malloc(sizeof(Matrix) * (layers - 1));

            for (int index = 0; index < layers - 1; index++){
                this->weights[index] = randomMatrix(shape[index + 1], shape[index]);
                this->biases[index]  = randomMatrix(shape[index + 1], 1);
            }
        }

        ForwardPropagationResult feed(Matrix I){
            ForwardPropagationResult result;

            result.A = (Matrix*) malloc(sizeof(Matrix) * layers);
            result.U = (Matrix*) malloc(sizeof(Matrix) * layers);

            result.A[0] = I;
            result.U[0] = I;

            Matrix A = I;
            for (int index = 0; index < this->layers - 1; index++){
                Matrix U = matmul(this->weights[index], A) + this->biases[index];

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
            Matrix dZ = R.A[this->layers - 1] - O;
            for (int index = layers - 1; index > 0; index--){
                int m = R.A[index].cols;

                Matrix dW = matmul(dZ, R.A[index - 1].transpose()).scale(1.0/float(m));
                Matrix dB = dZ.sum(1).scale(1.0/float(m));

                dZ = hadamard(matmul(this->weights[index - 1].transpose(), dZ), R.U[index - 1].map(&ReLU_prime));

                printf("dW (%i, %i)\n", dW.rows, dW.cols);
                dW.printf();
                printf("\n");
                printf("dB (%i, %i)\n", dB.rows, dB.cols);
                dB.printf();
                printf("\n");

                this->weights[index - 1] = this->weights[index - 1] - dW.scale(this->alpha);
                this->biases[index - 1] = this->biases[index - 1] - dB.scale(this->alpha);
            }
        }
};