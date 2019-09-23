#include "Matrix.cpp"
#include <cmath>

double sigmoid(double x) {
    return 1 / (1 + pow(exp(1), -x));
}

double sigmoid_derivative(double x) {
    return x * (1 - x);
}

double tanhh(double x) {
    return tanh(x);
}

double tanhh_derivative(double x) {
    return 1 - x * x;
}

//double tanhh_derivative(double x) {
//    return pow(1 / cosh(x), 2);
//}

// thought this will be more useful
// maybe remove it and use a simple vector for the number of neurons in each layer of the neural network
class Layer {
private:
    size_t nodes;
public:
    Layer(size_t nodes_) {
        nodes = nodes_;
    }
    size_t getNodes() {
        return nodes;
    }
};

// boom
class NeuralNetwork {
public:
//private:
    double learningRate = 0.15;
    std::vector<Layer> layers;
    std::vector<Matrix> weights;
    std::vector<Matrix> biases;
//public:

    NeuralNetwork& addLayer(Layer l) {
        std::cout << "Adding layer " << layers.size() << '\n';
        layers.push_back(l);
        return *this;
    }

    // create the weights and biases between each layer
    void build() {
        std::cout << "\nBuilding network...\n\n";
        for (size_t i = 0; i < layers.size() - 1; i++) {
            weights.push_back(Matrix(layers[i + 1].getNodes(), layers[i].getNodes()));
            biases.push_back(Matrix(layers[i + 1].getNodes(), 1));
            weights[i].setRandomValues();
            biases[i].setRandomValues();
        }
    }

    Matrix feedforward(const Matrix& input) {
        Matrix result = Matrix(input);
        // so first layer is input layer
        // and the rows of the input matrix has to match the number of nodes of the input layer
        // and also it should only have 1 column to do the matrix multiplication
        if (result.rows != layers[0].getNodes() || result.cols != 1) {
            throw "not good in feed forward";
        }

        // and for each of the next layers
        for (size_t i = 0; i < layers.size() - 1; i++) {
            // the "activation" matrix will be the current "input" matrix multiplied with
            // the weight matrix between the current and the next layer
            // plus the bias of the next layer
            result = Matrix::multiply(weights[i], result).add(biases[i]).map_function(tanhh);
        }
        // and return the result
        return result;
    }

    void backpropagate(const Matrix& input, Matrix target) {
        // basically feed forward, but keep the input for each layer ( starting with the 2nd one, ignore the input layer )
        Matrix result = Matrix(input);
        std::vector<Matrix> results {result};

        if (result.rows != layers[0].getNodes() || result.cols != 1) {
            throw "not good in back propagate";
        }

        for (size_t i = 0; i < layers.size() - 1; i++) {
            result = Matrix::multiply(weights[i], result).add(biases[i]);
            result.map_function(tanhh);
            results.push_back(result);
        }

        // back propagation of errors
        Matrix last_error;
        // starting from the last layer
        for (int i = results.size() - 1; i >= 1; i--) {
            Matrix error;
            // if we're at the last layer, get the difference target - output
            if ((size_t) i == results.size() - 1) {
                Matrix absolute_difference = target.subtract(results[i]);
                error = absolute_difference;
                //Matrix::multiply(absolute_difference, absolute_difference);
                // this is garbagio?
                // kinda wanna do mean squared error, but it spits out complete trash
            } else {
                // otherwise, the error will be the matrix product of the weight matrix and the last error
                error = Matrix::multiply(weights[i].transpose(), last_error);
            }
            // update the last error ( good comment right here )
            last_error = error;

            // gradient
            Matrix gradient = Matrix::map_function(results[i], tanhh_derivative);
            gradient.hadamard(error).multiply(learningRate);

            // deltas
            Matrix delta = Matrix::multiply(gradient, results[i - 1].transpose());

            // update weights and biases
            weights[i - 1].add(delta);
            biases[i - 1].add(gradient);
        }
    }

    void printStructure() {
        for (size_t i = 0; i < layers.size() - 1; i++) {
            std::cout << "Weights between layer " << i << " and layer " << i + 1 << ":\n";
            weights[i].print();
            std::cout << "\nBiases of layer " << i + 1 << ":\n";
            biases[i].print();
            std::cout << '\n';
        }
    }
};
