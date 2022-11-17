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
#include <iomanip>

using namespace std;

// Function for reading training data
tuple<vector<vector<double>>, vector<vector<int>>> read_data(string file)
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
    vector<vector<double>> input_data;
    vector<vector<int>> output_data;

    while(getline(inF, one_line))
    {
        int counter = 0;
        vector<double> input_tmp;
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

tuple<vector<vector<double>>, vector<vector<double>>> read_weights(string file)
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

    vector<vector<double>> w1;
    vector<vector<double>> w2;

    int counter = 0;
    while(getline(inF, one_line))
    {
        vector<double> tmp;


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

void print_trained_weights(string file, vector<vector<double>> w1, vector<vector<double>> w2)
{
    ofstream outF;
    outF.open(file);

    outF << w1[0].size() - 1 << " " << w1.size() << " " << w2.size() << "\n";
    outF << setprecision(3) << fixed;

    for (int i = 0; i < w1.size(); i++)
    {
        for (int j = 0; j < w1[i].size(); j++)
        {
            if (j == 0)
                outF << w1[i][j];
            else
                outF << " " << w1[i][j];
        }
        outF << "\n";
    }
    for (int i = 0; i < w2.size(); i++)
    {
        for (int j = 0; j < w2[i].size(); j++)
        {
            if (j == 0)
                outF << w2[i][j];
            else
                outF << " " << w2[i][j];
        }
        outF << "\n";
    }
}

//function for testing purposes only
void print_2d(vector<vector<double>> vec)
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
    double lr;

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

    tuple<vector<vector<double>>, vector<vector<int>>> input_output = read_data(training_data_file);
    tuple<vector<vector<double>>, vector<vector<double>>> w1_w2 = read_weights(initial_network_file);
    //print_2d(get<0>(input_output));

    neural_network nn(get<0>(input_output), get<0>(w1_w2), get<1>(w1_w2), get<1>(input_output), epochs, lr);
    tuple<vector<vector<double>>, vector<vector<double>>> new_w1_w2 = nn.train_network();

    print_trained_weights(output_file, get<0>(new_w1_w2), get<1>(new_w1_w2));

    return 0;
}