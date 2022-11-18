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
#include <cmath>

using namespace std;

neural_network::neural_network(vector<vector<double>> input_data, vector<vector<double>> weight_1, vector<vector<double>> weight_2, vector<vector<int>> outputs, int e, double lr)
{
    input = input_data;
    w1 = weight_1;
    w2 = weight_2;
    bool_outputs = outputs;
    epochs = e;
    learning_rate = lr;
}

neural_network::neural_network(vector<vector<double>> input_data, vector<vector<double>> weight_1, vector<vector<double>> weight_2, vector<vector<int>> outputs)
{
    input = input_data;
    w1 = weight_1;
    w2 = weight_2;
    bool_outputs = outputs;
}

double neural_network::sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

double neural_network::sigmoid_derivative(double x)
{
    return sigmoid(x) * (1 - sigmoid(x));
}

tuple<vector<vector<double>>, vector<vector<double>>> neural_network::train_network()
{

    // cout << w2.size();
    // cout << w1.size();

    // loop through epochs
    for(int e = 0; e < epochs; e++)
    {
        // loop through every sample one at a time
        for(int s = 0; s < input.size(); s++)
        {
            // ------ Forward Propagation ------
            // Get hidden layer
            vector<double> sample = input[s];
            vector<double> hidden_layer(w1.size());
            vector<double> hidden_layer_without_act(w1.size());
            for(int j = 0; j < w1.size(); j++)
            {
                double summation_input_to_hidden_j = w1[j][0] * (-1);
                for(int i = 0; i < sample.size(); i++)
                {
                    summation_input_to_hidden_j += sample[i] * w1[j][i+1];
                }
                hidden_layer_without_act[j] = summation_input_to_hidden_j;
                hidden_layer[j] = sigmoid(summation_input_to_hidden_j);
            }
            // Get output layer
            vector<double> output_layer(w2.size());
            vector<double> output_layer_without_activation(w2.size());
            for(int j = 0; j < w2.size(); j++)
            {
                double summation_hidden_to_output_j = w2[j][0] * (-1);
                for(int i = 0; i < hidden_layer.size(); i++)
                {
                    summation_hidden_to_output_j += hidden_layer[i] * w2[j][i+1];
                }
                output_layer_without_activation[j] = summation_hidden_to_output_j;
                output_layer[j] = sigmoid(summation_hidden_to_output_j);
            }

            // ------ Back Propagation ------ 
            // For delta storage
            vector<double> delta_j(w2.size());
            vector<double> delta_i(w1.size());
            vector<int> output = bool_outputs[s];
            // Get delta of output nodes
            for(int j = 0; j < output_layer.size(); j++)
            {
                delta_j[j] = sigmoid_derivative(output_layer_without_activation[j]) * (output[j] - output_layer[j]);
            }
            //Get delta of hidden nodes
            for(int i = 0; i < hidden_layer.size(); i++)
            {
                double tmp = 0;
                for(int j = 0; j < output_layer.size(); j++)
                {
                    tmp += w2[j][i+1] * delta_j[j];
                }
                delta_i[i] = sigmoid_derivative(hidden_layer_without_act[i]) * tmp;
            }

            // Update weights
            // For w2
            for(int j = 0; j < w2.size(); j++)
            {
                w2[j][0] = w2[j][0] + learning_rate * (-1) * delta_j[j];
                for(int i = 0; i < hidden_layer.size(); i++)
                {
                    w2[j][i+1] = w2[j][i+1] + learning_rate * hidden_layer[i] * delta_j[j];
                }
            }
            for(int j = 0; j < w1.size(); j++)
            {
                w1[j][0] = w1[j][0] + learning_rate * (-1) * delta_i[j];
                for(int i = 0; i < sample.size(); i++)
                {
                    w1[j][i+1] = w1[j][i+1] + learning_rate * sample[i] * delta_i[j];
                }
            }

        }
    }

    return make_tuple(w1, w2);
}

vector<vector<int>> neural_network::test_network()
{
    vector<vector<double>> complete_output;
    for(int s = 0; s < input.size(); s++)
    {
        // ------ Forward Propagation ------
        // Get hidden layer
        vector<double> sample = input[s];
        vector<double> hidden_layer(w1.size());
        for(int j = 0; j < w1.size(); j++)
        {
            double summation_input_to_hidden_j = w1[j][0] * (-1);
            for(int i = 0; i < sample.size(); i++)
            {
                summation_input_to_hidden_j += sample[i] * w1[j][i+1];
            }
            hidden_layer[j] = sigmoid(summation_input_to_hidden_j);
        }
        // Get output layer
        vector<double> output_layer(w2.size());
        for(int j = 0; j < w2.size(); j++)
        {
            double summation_hidden_to_output_j = w2[j][0] * (-1);
            for(int i = 0; i < hidden_layer.size(); i++)
            {
                summation_hidden_to_output_j += hidden_layer[i] * w2[j][i+1];
            }
            output_layer[j] = sigmoid(summation_hidden_to_output_j);
        }
        complete_output.push_back(output_layer);
    }
    
    // Find A B C D for each category (A is true positive, B is false positive, C is false negative, D is true negative)
    // Each column is the A, B, C, D value for that class respectively
    // Each row is a class
    vector<vector<int>> abcd;
    for(int i = 0; i < complete_output[0].size(); i++)
    {
        vector<int> abcd_one_class = {0, 0, 0, 0};

        for(int j = 0; j < complete_output.size(); j++)
        {
            if (complete_output[j][i] >= .5 && bool_outputs[j][i] == 1)
                abcd_one_class[0]++;
            else if (complete_output[j][i] >= .5 && bool_outputs[j][i] == 0)
                abcd_one_class[1]++;
            else if (complete_output[j][i] < .5 && bool_outputs[j][i] == 1)
                abcd_one_class[2]++;
            else
                abcd_one_class[3]++;
        }
        abcd.push_back(abcd_one_class);
    }
    
    
    return abcd;
}