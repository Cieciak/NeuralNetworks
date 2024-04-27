import numpy as np
import random

class Network:

    @staticmethod
    def ReLU(x: np.ndarray) -> np.ndarray: return np.maximum(x, 0)

    @staticmethod
    def ReLU_prime(x: np.ndarray) -> np.ndarray: return x > 0

    def __init__(self, shape: tuple[int, ...]):
        self.shape = shape

        self.weights: list[np.ndarray] = []
        self.biases: list[np.ndarray] = []
        self.alpha: float = 0.05

    def populate(self):
        for prev, next in zip(self.shape, self.shape[1:]):
            w = np.random.rand(next, prev) - .5
            self.weights.append(w)

            b = np.random.rand(next, 1) -.5
            self.biases.append(b)

    def __forward(self, I: np.ndarray) -> tuple[list[np.ndarray], list[np.ndarray]]:
        activated = [I,]
        unactivated = [I]

        A = I
        for index in range(len(self.shape) - 1):
            U = self.weights[index] @ A + self.biases[index]
            A = self.ReLU(U)

            activated.append(A)
            unactivated.append(U)

        return activated, unactivated
    
    def forward(self, I: np.ndarray) -> np.ndarray:
        A, _ = self.__forward(I)
        return A[-1]
    
    def back(self, I: np.ndarray, O: np.ndarray):
        A, U = self.__forward(I)
        dZ = A[-1] - O
        for index in range(len(self.shape) - 1, 0, -1):
            _, m = A[index].shape 

            #print(f'dZ[{index}]={dZ}')
            #print(f'm[{index}]={m}')

            dW = 1/m * dZ @ A[index-1].T
            dB = 1/m * np.sum(dZ, axis = 1)

            #print(f'dW[{index}]={dW}')
            #print(f'dB[{index}]={dB}')

            dZ = self.weights[index-1].T @ dZ * self.ReLU_prime(U[index - 1])

            #print(self.alpha * dB)

            self.weights[index-1] -= self.alpha * dW
            self.biases[index-1] = self.biases[index-1] - self.alpha * dB.reshape((-1, 1))

            #for b in self.biases:
            #    print(b.shape, b, dB, sep='\n', end='\n\n')

def convert(n: int, *, c: int = 8):
    bits = []
    for _ in range(c): n, r = divmod(n, 2); bits += [float(r),]
    return bits

def generate(size: int, bsize: int = 10):

    data = []
    for index in range(2 ** size):
        if index % 7 == 0:
            data.append([convert(index), [1.0, 0.0]])
        else:
            data.append([convert(index), [0.0, 1.0]])

    random.shuffle(data)
    
    batches = []
    for index in range(bsize):
        batches.append(
            data[index::bsize]
        )

    pairs = []
    for b in batches:
        Im = []
        Om = []
        for I, O in b:
            Im += [I,]
            Om += [O,]

        pairs.append(
            [np.array(Im).T, np.array(Om).T]
        )

    return pairs 

if __name__ == '__main__':
    n = Network((8, 10, 2))
    n.populate()

    data = generate(8, 20)

    I, O = data[1]
    E = n.forward(I)

    print(f'Input: \n{I}')
    print(f'Target: \n{O}')
    print(f'Result: \n{E}')
    print()
    for _ in range(10000):
        for I, O in data:
            n.back(I, O)

    I, O = data[1]
    E = n.forward(I)

    print(f'Input: \n{I}')
    print(f'Target: \n{O}')
    print(f'Result: \n{E}')
    print()