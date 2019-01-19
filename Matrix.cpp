#include <iostream>
#include <vector>
#include <ctime>

// beware, this class is completely garbage as well, but it does the job

// TODO: refactor all this garbage and maybe add multithreading to the matrix operations
// really necessary

class Matrix {
public:
//private:
    size_t rows;
    size_t cols;
    std::vector<std::vector<double>> values;
//public:

    Matrix() {
        // should be here for academical purposes
    }

    Matrix(size_t rows_, size_t cols_) {
        // set the number of rows and columns of the matrix
        rows = rows_;
        cols = cols_;
        // resize the values array
        values.resize(rows);
        // and also resize each array ( row ) of the 2d values array
        for (auto &v : values) {
            v.resize(cols);
        }
    }

    Matrix(std::vector<std::vector<double>>& vals) {
        rows = vals.size();
        cols = vals[0].size();
        values = vals;
    }

    Matrix& setRandomValues() {
        srand(time(NULL) + rand() % 100);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                values[i][j] = (double) (rand() % 10000 + 1) / 10000;
            }
        }
        return *this;
    }

    Matrix(const Matrix& other) {
        *this = other;
    }

    ~Matrix() {
//        std::cout << "Destroyed...\n";
    }

    Matrix& setValues(const std::vector<std::vector<double>>& newValues) {
        if (rows != newValues.size()) {
            throw "Number of rows doesn't match.";
        } else if (cols != newValues[0].size()) {
            throw "Number of columns doesn't match.";
        } else {
            values = newValues;
        }
        return *this;
    }

    Matrix transpose() {
        Matrix transposed = Matrix(cols, rows);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                transposed.values[j][i] = values[i][j];
            }
        }
        return transposed;
    }

    // add one matrix to this matrix
    Matrix& add(const Matrix& other) {
        // if the number of rows and the number of columns doesn't match, OOPSIE!
        if (rows != other.rows) {
            throw "Number of rows doesn't match.";
        } else if (cols != other.cols) {
            throw "Number of columns doesn't match.";
        } else {
            // otherwise, just add element-wise
            for (size_t i = 0; i < rows; i++) {
                for (size_t j = 0; j < cols; j++) {
                    values[i][j] += other.values[i][j];
                }
            }
        }
        return *this;
    }

    // hadamard product of two matrices
    Matrix& hadamard(const Matrix& other) {
        // if the number of rows and the number of columns doesn't match, OOPSIE!
        if (rows != other.rows) {
            throw "Number of rows doesn't match.";
        } else if (cols != other.cols) {
            throw "Number of columns doesn't match.";
        } else {
            // otherwise, just add element-wise
            for (size_t i = 0; i < rows; i++) {
                for (size_t j = 0; j < cols; j++) {
                    values[i][j] *= other.values[i][j];
                }
            }
        }
        return *this;
    }

    // add a scalar to this matrix ( element-wise )
    Matrix& add(double scalar) {
        for (auto& row : values) {
            for (auto& col : row) {
                col += scalar;
            }
        }
        return *this;
    }

    // subtract a scalar from this matrix ( probably completely unnecessary )
    Matrix& subtract(double scalar) {
        // use the above "add" method with negative scalar, ez.
        return scalar < 0 ? add(scalar) : add(-scalar);
    }

    // subtract a matrix from this matrix
    Matrix& subtract(const Matrix& other) {
        // so multiply the other matrix with -1 and add it to this matrix
        return add(Matrix(other).multiply(-1));
    }

    // multiply this matrix with another matrix
    Matrix& multiply(const Matrix& other) {
        // if this matrix is MxN, then the other matrix has to be NxP
        // so the cols of this matrix has to be equal to the rows of the other matrix
        if (cols != other.rows) {
            throw "Can't multiply; num of cols not matching with num of rows";
        } else {
            // the new matrix will have this matrix's number of rows and the other matrix's number of cols
            Matrix result(rows, other.cols);
            // and now perform matrix multiplication
            for(size_t i = 0; i < rows; i++) {
                for(size_t j = 0; j < other.cols; j++) {
                    for(size_t k = 0; k < cols; k++) {
                        result.values[i][j] += values[i][k] * other.values[k][j];
                    }
                }
            }
            *this = result;
            return *this;
        }
    }

    // multiply this matrix with a scalar ( element-wise )
    Matrix& multiply(double scalar) {
        for (auto& row : values) {
            for (auto& col : row) {
                col *= scalar;
            }
        }
        return *this;
    }

    static Matrix multiply(const Matrix& first, const Matrix& second) {
        if (first.cols != second.rows) {
            throw "Can't multiply; num of cols not matching with num of rows";
        } else {
            // the new matrix will have this matrix's number of rows and the other matrix's number of cols
            Matrix result(first.rows, second.cols);
            // and now perform matrix multiplication
            for(size_t i = 0; i < first.rows; i++) {
                for(size_t j = 0; j < second.cols; j++) {
                    for(size_t k = 0; k < first.cols; k++) {
                        result.values[i][j] += first.values[i][k] * second.values[k][j];
                    }
                }
            }
            return result;
        }
    }

    // get each element of the matrix through a given function
    Matrix& map_function(double activation(double)) {
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                values[i][j] = activation(values[i][j]);
            }
        }
        return *this;
    }

    // same, but static
    static Matrix map_function(const Matrix& matrix, double activation(double)) {
        Matrix result(matrix);
        for (size_t i = 0; i < result.rows; i++) {
            for (size_t j = 0; j < result.cols; j++) {
                result.values[i][j] = activation(result.values[i][j]);
            }
        }
        return result;
    }

    // print the matrix
    void print() {
        for (auto& row : values) {
            for (auto& col : row) {
                std::cout << col << "\t";
            }
            std::cout << "\n";
        }
    }

    // returns the 2d values array... idk if this was needed, really.
    const std::vector<std::vector<double>>& getInternalValues() const {
        return values;
    }

};
