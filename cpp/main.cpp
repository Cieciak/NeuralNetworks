#include "network.cpp"

int power(int base, int exponent){
    if (exponent == 0){return 1;}
    else {return power(base, exponent - 1) * base;}
}

Matrix convert(int number, int width){
    Matrix M = Matrix(width, 1, 0.0);
    for (int row = 0; row < width; row++){
        float bit = number % 2;
        number    = number / 2;
        M.setElement(
            row,
            0,
            bit
        );
        
    }
    return M;
}

Matrix oneHot(int place, int width){
    Matrix M = Matrix(width, 1, 0.0);
    for (int index = 0; index < width; index++){
        if (index == place){ M.setElement(index, 0, 1.0); }
        else { M.setElement(index, 0, 0.0); }
    }
    return M;
}

struct DataBatch {
    int block;
    Matrix* input;
    Matrix* target;
};

DataBatch generate(int size){
    int total = power(2, size);
    std::printf("Amount of data samples: %i\n", total);
    
    int batch_count = power(2, size / 2);
    std::printf("Batches count: %i\n", batch_count);

    int batch_size = total / batch_count;
    std::printf("Batches count: %i\n", batch_size);

    Matrix* data   = (Matrix*) malloc(sizeof(Matrix) * total);
    Matrix* result = (Matrix*) malloc(sizeof(Matrix) * total);
    for (int index = 0; index < total; index++){
        data[index] = convert(index, size);
        if (index % 7 == 0){result[index] = oneHot(0, 2);}
        else {result[index] = oneHot(1, 2);}
    }

    DataBatch batch;
    batch.block = batch_count;
    batch.input  = (Matrix*) calloc(batch_count, sizeof(Matrix));
    batch.target = (Matrix*) calloc(batch_count, sizeof(Matrix));
    for (int index = 0; index < batch_count; index++){
        Matrix d = data[index * batch_size];
        Matrix r = result[index * batch_size];
        for (int offset = 1; offset < batch_size; offset++){
            d = concat(d, data[index * batch_size + offset]);
            r = concat(r, result[index * batch_size + offset]);
        }
        batch.input[index] = d;
        batch.target[index] = r;
    }
    std::printf("Data generation complete\n");
    return batch;
}

int main(){
    init();

    int layers = 4;
    int shape[layers] = {9,25,5,2};

    int* s = shape;

    Network N = Network(layers, s, 0.05);

    DataBatch B = generate(9);

    for (int iteration = 0; iteration < 100; iteration++){
        std::printf("Iteration number: %i\n", iteration);
        for (int block = 0; block < B.block; block++){
            //std::printf("Block %i\n", block);
            //std::printf("Block input:\n");
            //B.input[block].print();
            N.backward(B.input[block], B.target[block]);
        }
    }

    std::printf("T1 = \n");
    Matrix T1 = convert(0x07, 9);
    T1.printf();
    std::printf("\n");
    Matrix R = N.forward(T1);
    R.printf();
    std::printf("\n");

    std::printf("T2 = \n");
    T1 = convert(0x09, 9);
    T1.printf();
    std::printf("\n");
    R = N.forward(T1);
    R.printf();
}