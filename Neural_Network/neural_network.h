#ifndef _NEURAL_NETWORK_H
#define _NEURAL_NETWORK_H

#include <vector>
#include <string>
#include <tuple>
#include <thread>
#include <chrono>

using namespace std;

class neural_network {

    public:
        // Constructor for training
        neural_network(vector<vector<double>> input_data, vector<vector<double>> weight_1, vector<vector<double>> weight_2, vector<vector<int>> outputs, int e, double lr);

        // Constructor for testing
        neural_network(vector<vector<double>> input_data, vector<vector<double>> weight_1, vector<vector<double>> weight_2, vector<vector<int>> outputs);

        // Trains neural network using pseudocode and updates the weights
        // Returns a tuple of updated weights after training is completed <w1, w2>
        tuple<vector<vector<double>>, vector<vector<double>>> train_network();

        // Gets statistics on the performance of the network
        // Returns a 2d vector with each row being a class
        // Column 1 -> A (correctly identifies positive)
        // Column 2 -> B (false positive)
        // Column 3 -> C (false negative)
        // Column 4 -> D (correctly identifies negative)
        vector<vector<int>> test_network();

    private:
        // Network properties for both testing and training
        vector<vector<double>> input;
        vector<vector<double>> w1;
        vector<vector<double>> w2;
        vector<vector<int>> bool_outputs;

        // Network properties for training only
        int epochs;
        double learning_rate;

        // Helper functions for the activation function and its derivative
        double sigmoid(double x);
        double sigmoid_derivative(double x);
};
#endif //_NEURAL_NETWORK_H