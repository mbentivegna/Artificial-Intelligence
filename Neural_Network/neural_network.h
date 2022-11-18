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

        tuple<vector<vector<double>>, vector<vector<double>>> train_network();

        vector<vector<int>> test_network();

    private:
        vector<vector<double>> input;
        vector<vector<double>> w1;
        vector<vector<double>> w2;
        vector<vector<int>> bool_outputs;
        int epochs;
        double learning_rate;

        double sigmoid(double x);
        double sigmoid_derivative(double x);

};
#endif //_NEURAL_NETWORK_H