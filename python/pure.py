import random
import struct

class Matrix:

    @classmethod
    def fromArray(cls, rows: int, columns: int, data: list[float]):
        M = cls(rows, columns)

        for i in range(rows):
            for j in range(columns):
                M[i, j] = data[i * columns + j]
        
        return M
    
    @classmethod
    def fromTable(cls, table: list[list[tuple]]):
        rows = len(table)
        cols = len(table[0])

        print(table)
        M = Matrix(rows = rows, columns = cols, init = 0.0)

        for i in range(rows):
            for j in range(cols):
                M[i,j] = table[i][j]

        return M

    @classmethod
    def fromBytes(cls, rows: int, columns: int, data: bytearray):
        M = cls(rows, columns)

        for j in range(columns):
            for i in range(rows):
                M[i, j] = struct.unpack('f', data[:4])[0] 
                data = data[4:]
        return M

    @classmethod
    def random(cls, rows: int, columns: int):
        M = cls(rows, columns)

        for i in range(rows):
            for j in range(columns):
                M[i, j] = random.random() - .5

        return M

    def __init__(self, rows: int, columns: int, *, init: float = 0.0):
        self.rows    = rows
        self.columns = columns
        self.__data  = [init for _ in range(self.rows * self.columns)]

    def __getitem__(self, index):
        i, j = index
        return self.__data[i * self.columns + j]
    
    def __setitem__(self, index, value):
        i, j = index
        self.__data[i * self.columns + j] = value

    def __repr__(self) -> str:
        output = ''
        for i in range(self.rows):
            output += '|'
            for j in range(self.columns):
                output += f' {self[i,j]:8.5f} '
            output += '|\n'
        return output

    def __add__(U, V):
        if U.rows != V.rows: raise IndexError('Cannot add two matrixes')
        if V.columns == 1:
            M = Matrix(rows = U.rows, columns = U.columns, init = 0.0)
            for i in range(U.rows):
                for j in range(U.columns):
                    M[i,j] = U[i,j] + V[i, 0]
        else:
            M = Matrix(rows = V.rows, columns = U.columns, init = 0.0)
            for i in range(U.rows):
                for j in range(V.columns):
                    M[i, j] = U[i, j] + V[i, j]
        return M
    
    def __sub__(U, V):
        if U.rows != V.rows or U.columns != V.columns: raise IndexError('Matrix sizes are different')

        M = Matrix(rows = V.rows, columns = U.columns, init = 1.0)
        for i in range(U.rows):
            for j in range(V.columns):
                M[i, j] = U[i, j] - V[i, j]
        
        return M       

    def __mul__(U, c):        
        if type(c) == Matrix:
            M = Matrix(rows = U.rows, columns = U.columns)

            for i in range(U.rows):
                for j in range(U.columns):
                    M[i,j] = U[i,j] * c[i,j]
            return M
        else:
            M = Matrix(rows = U.rows, columns = U.columns)

            for i in range(U.rows):
                for j in range(U.columns):
                    M[i,j] = U[i,j] * c

            return M

    def __rmul__(U,c): return U.__mul__(c)

    def __matmul__(U, V):
        if U.columns != V.rows: raise IndexError('Sizes doesn\'t match')

        k, l, l , m = U.rows, U.columns, V.rows, V.columns

        M = Matrix(rows = k, columns = m)
        
        for i in range(M.rows):
            for j in range(M.columns):
                M[i, j] = sum(
                    U[i, x] * V[x, j] for x in range(l)
                )

        return M

    @property
    def T(self):
        M = Matrix(rows = self.columns, columns = self.rows)

        for i in range(self.rows):
            for j in range(self.columns):
                M[j, i] = self[i, j]

        return M

    def map(self, func):
        M = Matrix(rows = self.rows, columns = self.columns)

        for i in range(self.rows):
            for j in range(self.columns):
                M[i,j] = func(self[i, j])
        
        return M

    def serialize(self):
        buffer = bytearray()

        for j in range(self.columns):
            for i in range(self.rows):
                buffer += struct.pack('f', self[i,j])
        return buffer

    def sum(self, axis: int = 0):

        if axis == 0:
            M = Matrix(rows = 1, columns = self.columns, init = 0.0)
            for j in range(self.columns):
                M[0, j] = sum(self[i, j] for i in range(self.rows))
        elif axis == 1:
            M = Matrix(rows = self.rows, columns = 1, init = 0.0)
            for i in range(self.rows):
                M[i, 0] = sum(self[i, j] for j in range(self.columns))
        
        return M

    
class Network:

    @classmethod
    def getFromFile(cls, path: str):
        with open(path, 'rb') as file: data = bytearray(file.read())

        shape: list[int] = [struct.unpack('i', data[:4])[0],]
        while shape[-1]:
            data = data[4:]
            shape += struct.unpack('i', data[:4])

        data = data[4:]
        shape = shape[:-1]

        weights = []
        for rows, cols in zip(shape, shape[1:]):
            weights.append(
                Matrix.fromBytes(cols, rows, data[:4 * rows * cols])
            )
            data = data[4 * rows * cols:]

        vectors = []
        for cols in shape[1:]:
            vectors.append(
                Matrix.fromBytes(cols, 1, data[:4 * cols])
            )
            data = data[4 * cols:]

        n = cls(tuple(shape))
        n.weights = weights
        n.biases = vectors

        return n
    
    @staticmethod
    def ReLU(X: Matrix) -> Matrix: return X.map(lambda x: max(0, x))

    @staticmethod
    def ReLU_prime(X: Matrix) -> Matrix: return X.map(lambda x: x > 0)

    def __init__(self, shape: tuple[int, ...]):
        self.shape = shape

        self.weights: list[Matrix] = []
        self.biases: list[Matrix] = []
        self.alpha: float = 0.05

    def populate(self):
        for prev, next in zip(self.shape, self.shape[1:]):
            w = Matrix.random(next, prev)
            self.weights.append(w)

            b = Matrix.random(next, 1)
            self.biases.append(b)

    def __forward(self, I: Matrix) -> tuple[list[Matrix], list[Matrix]]:
        activated = [I,]
        unactivated = [I,]

        A = I
        for index in range(len(self.shape) - 1):
            U = self.weights[index] @ A + self.biases[index]
            A = self.ReLU(U)

            activated.append(A)
            unactivated.append(U)

        return activated, unactivated
    
    def forward(self, I: Matrix) -> Matrix:
        A, _ = self.__forward(I)
        return A[-1]
    
    def back(self, I: Matrix, O: Matrix):
        A, U = self.__forward(I)
        dZ = A[-1] - O
        for index in range(len(self.shape) - 1, 0, -1):
            m = A[index].columns

            dW = 1/m * dZ @ A[index - 1].T
            dB = 1/m * dZ.sum(axis = 1)

            dZ = self.weights[index - 1].T @ dZ * self.ReLU_prime(U[index - 1])

            self.weights[index - 1] = self.weights[index - 1] - self.alpha * dW
            self.biases[index - 1] = self.biases[index - 1] - self.alpha * dB

    def serialize(self):
        buffer = bytearray()

        for size in self.shape:
            buffer += struct.pack('i', size)
        buffer += struct.pack('i', 0x00)

        for matrix in self.weights:
            buffer += matrix.serialize()

        for vector in self.biases:
            buffer += vector.serialize()

        return buffer
    
    def putToFile(self, path: str):
        with open(path, 'wb') as file: file.write(self.serialize())

def convert(n: int, *, c: int = 8):
    bits = []
    for _ in range(c): n, r = divmod(n, 2); bits += [float(r),]
    return bits

def generate(size: int, bsize: int = 10):
    data = []
    for index in range(2 ** size):
        if index % 9 == 0:
            data.append(
                [convert(index, c=size), [1.0, 0.0]]
            )
        else:
            data.append(
                [convert(index, c=size), [0.0, 1.0]]
            )
    
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
            [Matrix.fromTable(Im).T, Matrix.fromTable(Om).T]
        )

    return pairs

if __name__ == '__main__':
    n = Network((8, 20, 10, 2))
    n.populate()

    data = generate(8, 20)

    I, O = data[1]
    E = n.forward(I)

    print(f'Input: \n{I}')
    print(f'Target: \n{O}')
    print(f'Result: \n{E}')
    print(f'Diff:\n{O - E}')
    print(f'Accuracy:\n{(O - E).sum(axis=1) * (1 / E.columns)}')
    print()
    for index in range(100):
        print(f'Iteration: {index}')
        for I, O in data:
            n.back(I, O)

    I, O = data[1]
    E = n.forward(I)

    print(f'Input: \n{I}')
    print(f'Target: \n{O}')
    print(f'Result: \n{E}')
    print(f'Diff:\n{O - E}')
    print(f'Accuracy:\n{(O - E).sum(axis=1) * (1 / E.columns)}')
    print()

    n.putToFile('test.lbt')

    n = Network.getFromFile('test.lbt')
    I, O = data[1]
    E = n.forward(I)

    print(f'Input: \n{I}')
    print(f'Target: \n{O}')
    print(f'Result: \n{E}')
    print(f'Diff:\n{O - E}')
    print(f'Accuracy:\n{(O - E).sum(axis=1) * (1 / E.columns)}')
    print()