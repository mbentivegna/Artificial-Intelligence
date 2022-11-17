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
        neural_network(vector<vector<float>> input_data, vector<vector<float>> weight_1, vector<vector<float>> weight_2, vector<vector<int>> outputs, int e, int lr);

    private:
        vector<vector<float>> input;
        vector<vector<float>> w1;
        vector<vector<float>> w2;
        vector<vector<int>> bool_outputs;
        int epochs;
        float learning_rate;
};
#endif //_NEURAL_NETWORK_H