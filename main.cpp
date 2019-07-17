#include "NeuralNetwork.cpp"

#include <chrono>

// i removed a comment! AND NOW I'M DOING GIT DIFF!

int main() {

    // declare the network
    NeuralNetwork nn = NeuralNetwork();

    // add a few layers
    // too many neurons and everything is gonna explode. beware.
    // i mean, the network will learn garbage. the results are gonna be nonsense, since the error will be lost
    // or some stuff like that
    nn.addLayer(Layer(2)).addLayer(Layer(3)).addLayer(Layer(1));

    // build the network
    nn.build();

    // for example, make it learn XOR

    // vector-ception
    std::vector<std::vector<std::vector<double>>> inputs  = {
        {{0}, {0}},
        {{0}, {1}},
        {{1}, {0}},
        {{1}, {1}}
    };

    std::vector<std::vector<std::vector<double>>> targets  = {
        {{0}},
        {{1}},
        {{1}},
        {{0}}
    };

    // record start time
    auto start = std::chrono::system_clock::now();
    std::cout << "Started learning!\n\n";

    // train the network!
    // for let's say, 50000 times
    const int epochs = 50000;
    for (int i = 0; i < epochs; i++) {
        // pick a random input and target
        int idx = rand() % 4;
        // and train the network on this input
        nn.backpropagate(inputs[idx], targets[idx]);
        // and print the current progress... or some stuff like this.
        if (i % (epochs / 10) == 0) {
            double current = 100.0 * i / epochs;
            std::cout << "Current progress: " << current << "%\n";
        }
    }

    // record end time
    auto end = std::chrono::system_clock::now();
    std::cout << "Finished learning!\n\n";

    // difference
    std::chrono::duration<double> diff = end - start;

    std::cout << "Time spent learning : " << diff.count() << " seconds\n\n";

    std::cout << "Results after training: \n\n";

    // and now test the thing
    // it should print something close to 0, 1, 1, 0
    for (size_t i = 0; i < inputs.size(); i++) {
        nn.feedforward(inputs[i]).print();
    }

    return 0;
}
