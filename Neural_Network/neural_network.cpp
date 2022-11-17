/*
Michael Bentivegna
Artificial Intelligence 
Neural Network
*/

#include "neural_network.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <thread>
#include <chrono>
#include <ctime>
#include <algorithm>

using namespace std;

neural_network::neural_network(vector<vector<float>> input_data, vector<vector<float>> weight_1, vector<vector<float>> weight_2, vector<vector<int>> outputs, int e, int lr)
{
    input = input_data;
    w1 = weight_1;
    w2 = weight_2;
    bool_outputs = outputs;
    epochs = e;
    learning_rate = lr;
}