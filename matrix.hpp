
#include <iostream>
#include <type_traits>
#include <cstring>

template<typename T, size_t N, size_t M>
class Matrix;

// using size_t = unsigned int;


template<typename T, size_t N, size_t M>
class Matrix {

    T** data;

public:

    Matrix();
    
    Matrix(const Matrix& other);

    ~Matrix();

    Matrix<T, N, M> operator +(const Matrix<T, N, M>& other) const;

    Matrix<T, N, M>& operator +=(const Matrix<T, N, M>& other);

    template<size_t new_M>
    Matrix<T, N, new_M> operator *(const Matrix<T, M, new_M>& other) const;
    
    Matrix<T, N, N>& operator *=(const Matrix<T, N, N>& other);

    Matrix<T, N, M> operator *(const T& elem) const;
    
    Matrix<T, N, M>& operator *=(const T& elem);

    void operator ++();

    T determinant() const;

    T& operator()(size_t n, size_t m);

    const T& operator()(size_t n, size_t m) const;

    template<typename U, size_t N1, size_t M1>
    friend std::ostream& operator<<(std::ostream& cout, const Matrix<U, N1, M1>& matrix);

    template<typename U, size_t N1, size_t M1>
    friend std::istream& operator>>(std::istream& cin, Matrix<U, N1, M1>& matrix);

    // friend class Matrix<T, N - 1, N - 1>;

};

template<typename T>
class Matrix<T, 1, 1> {
    T** data;

public:
    Matrix() {
        data = new T*(new T);
    }

    ~Matrix() {
        delete data[0];
        delete data;
    }

    T determinant() const {
        return data[0][0];
    }

    T& operator()(size_t n, size_t m) {
        return data[n][m];
    }

    const T& operator()(size_t n, size_t m) const {
        return data[n][m];
    }
};


template<typename T, size_t N, size_t M>
Matrix<T, N, M>::Matrix() {
    data = new T*[N];
    for (size_t i = 0; i < M; ++i) {
        data[i] = new T[M];
    }
}

template<typename T, size_t N, size_t M>
Matrix<T, N, M>::Matrix(const Matrix& other) {
    data = new T*[N];

    for (size_t i = 0; i < N; ++i) {
        data[i] = new T[N];
        memcpy(data[i], other.data[i], M*sizeof(T));  
    }
}

template<typename T, size_t N, size_t M>
Matrix<T, N, M>::~Matrix() {
    if (!data) return;
    for (size_t i = 0; i < N; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

template<typename T, size_t N, size_t M>
Matrix<T, N, M> Matrix<T, N, M>::operator +(const Matrix<T, N, M>& other) const {
    Matrix<T, N, M> matrix;
    T** new_data = new T*[N];
    for (size_t i = 0; i < N; ++i) {
        new_data[i] = new T[M];
        for (size_t j = 0; j < M; ++j) {
            new_data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    matrix.data = new_data;
    return matrix;
}

template<typename T, size_t N, size_t M>
Matrix<T, N, M>& Matrix<T, N, M>::operator +=(const Matrix<T, N, M>& other) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            data[i][j] += other.data[i][j];
        }
    }
    return *this;
}

template<typename T, size_t N, size_t M>
template<size_t new_M>
Matrix<T, N, new_M> Matrix<T, N, M>::operator *(const Matrix<T, M, new_M>& other) const {
    Matrix<T, N, new_M> matrix;
    
    for (size_t i = 0; i < N; ++ i) {
        for (size_t j = 0; j < new_M; ++j) {
            T sum = T();
            for (size_t k = 0; k < M; ++k) {
                sum += data[i][k] * other(k, j);
            }
            matrix(i, j) = sum;
        }
    }
    return matrix;
}

template<typename T, size_t N, size_t M>
Matrix<T, N, N>& Matrix<T, N, M>::operator *=(const Matrix<T, N, N>& other) {
    if (N != M) throw std::length_error("NxN required");

    T* tmp_row = new T[N];
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            T sum = T();
            for (size_t k = 0; k < N; ++k) {
                sum += data[i][k] * other(k, j);
            }
            tmp_row[j] = sum;
        }
        memcpy(data[i], tmp_row, N*sizeof(T));
    }
    delete[] tmp_row;
    return *this;
}

template<typename T, size_t N, size_t M>
Matrix<T, N, M> Matrix<T, N, M>::operator *(const T& elem) const {
    Matrix<T, N, M> matrix;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            matrix(i, j) = data[i][j]*elem;
        }
    }
    return matrix;
}

template<typename T, size_t N, size_t M>
Matrix<T, N, M>& Matrix<T, N, M>::operator *=(const T& elem) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            data[i][j] *= elem;
        }
    }
    return *this;
}

template<typename T, size_t N, size_t M>
void Matrix<T, N, M>::operator ++() {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            ++data[i][j];
        }
    }
}

template<typename T, size_t N, size_t M>
T Matrix<T, N, M>::determinant() const {
    if (N != M) throw std::length_error("NxN required");

    T res = T();

    // разложить по столбцу
    Matrix<T, N-1, N-1> tmp;

    for (size_t i = 0; i < N; ++i) {
        bool flag = false;
        for (size_t j = 0; j < N; ++j) {
            if (i == j) {
                flag = true;
                continue;
            }
            // tmp.data[j - flag] = data[i] + 1;
            for (size_t k = 0; k < N - 1; ++k) {
                tmp(j - flag, k) = data[j][k + 1];
            }
        }
        res += data[i][0] * tmp.determinant() * (i % 2 ? -1 : 1);
    }
    return res;
}

template<typename T, size_t N, size_t M>
T& Matrix<T, N, M>::operator()(size_t n, size_t m) {
    return data[n][m];
}

template<typename T, size_t N, size_t M>
const T& Matrix<T, N, M>::operator()(size_t n, size_t m) const {
    return data[n][m];
}


template<typename T, size_t N, size_t M>
std::ostream& operator<<(std::ostream& cout, const Matrix<T, N, M>& matrix) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            cout << matrix(i, j) << ' ';
        }
        cout << '\n';
    }
    return cout;
}

template<typename T, size_t N, size_t M>
std::istream& operator>>(std::istream& cin, Matrix<T, N, M>& matrix) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            cin >> matrix(i,j);
        }
    }
    return cin;
}

