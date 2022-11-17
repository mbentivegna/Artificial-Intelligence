/*
Michael Bentivegna
Artificial Intelligence 
Neural Network
*/

#include "neural_network.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include <cctype>
#include <ctime>
#include <vector>
#include <tuple>


using namespace std;

// Function for reading training data
tuple<vector<vector<float>>, vector<vector<int>>> read_data(string file)
{
    ifstream inF;
    inF.open(file);

    string one_line;
    getline(inF, one_line);  

    istringstream iss(one_line);
    string word;
    int input_nodes, output_nodes, num_samples;
    
    int num = 0;
    while (iss >> word) 
    {  
        if (num == 0)
            num_samples = stoi(word);
        if (num == 1)
            input_nodes = stoi(word);
        if (num == 2)
            output_nodes = stoi(word);
        num++;
    };

    // Make data and output 2d vectors with specified sizes
    vector<vector<float>> input_data;
    vector<vector<int>> output_data;

    while(getline(inF, one_line))
    {
        int counter = 0;
        vector<float> input_tmp;
        vector<int> output_tmp;

        istringstream iss(one_line);
        string word;

        while (iss >> word)
        {
            if (counter < input_nodes)
                input_tmp.push_back(stof(word));
            else
                output_tmp.push_back(stoi(word));
            counter++;
        }
        input_data.push_back(input_tmp);
        output_data.push_back(output_tmp);
    }

    return make_tuple(input_data, output_data);

}

tuple<vector<vector<float>>, vector<vector<float>>> read_weights(string file)
{
    ifstream inF;
    inF.open(file);

    string one_line;
    getline(inF, one_line);  

    istringstream iss(one_line);
    string word;
    int input_layer, hidden_layer, output_layer;

    int num = 0;
    while (iss >> word) 
    {  
        if (num == 0)
            input_layer = stoi(word);
        if (num == 1)
            hidden_layer = stoi(word);
        if (num == 2)
            output_layer = stoi(word);
        num++;
    };

    vector<vector<float>> w1;
    vector<vector<float>> w2;

    int counter = 0;
    while(getline(inF, one_line))
    {
        vector<float> tmp;


        istringstream iss(one_line);
        string word;


        while (iss >> word)
        {
            tmp.push_back(stof(word));
        }

        if (counter < hidden_layer)
            w1.push_back(tmp);
        else    
            w2.push_back(tmp);

        counter++;
    }
    return make_tuple(w1, w2);
}
//function for testing purposes only
void print_2d(vector<vector<float>> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        for (int j = 0; j < vec[i].size(); j++)
        {
            cout << vec[i][j] << " ";
        }
        cout << "\n";
    }
}

//Mission control
int main() 
{
    string initial_network_file, training_data_file, output_file;
    int epochs;
    float lr;

    cout << "Please input the name of the initial neural network file: \n";
    cin >> initial_network_file;

    cout << "Please input the name of the training data file: \n";
    cin >> training_data_file;

    cout << "Please input the name of the output file: \n";
    cin >> output_file;

    cout << "How many training epochs would you like the model to run?  \n";
    cin >> epochs;

    cout << "What do you want the learning rate of the model to be? \n";
    cin >> lr;

    tuple<vector<vector<float>>, vector<vector<int>>> input_output = read_data(training_data_file);
    tuple<vector<vector<float>>, vector<vector<float>>> w1_w2 = read_weights(initial_network_file);
    //print_2d(get<0>(input_output));

    return 0;
}