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

double overall_accuracy(vector<int> abcd)
{
    return (double)(abcd[0] + abcd[3]) / (abcd[0] + abcd[1] + abcd[2] + abcd[3]);
}

double precision(vector<int> abcd)
{
    return (double)(abcd[0]) / (abcd[0] + abcd[1]);
}

double recall(vector<int> abcd)
{
    return (double)(abcd[0]) / (abcd[0] + abcd[2]);
}

double f1(vector<int> abcd)
{
    return (2 * precision(abcd) * recall(abcd)) / (precision(abcd) + recall(abcd));
}

void print_results(string file, vector<vector<int>> abcd)
{
    ofstream outF;
    outF.open(file);

    outF << setprecision(3) << fixed;
    
    // Columns: overall acc, precision, recall, F1
    // Each row is its own class
    vector<vector<double>> stats;

    // For Micro calculations
    vector<int> total_vals = {0, 0, 0, 0};

    for(int i = 0; i < abcd.size(); i++)
    {
        for(int j = 0; j < abcd[0].size(); j++)
        {
            outF << abcd[i][j] << " ";
            total_vals[j] += abcd[i][j];
        }
        stats.push_back({overall_accuracy(abcd[i]), precision(abcd[i]), recall(abcd[i]), f1(abcd[i])});

        outF << overall_accuracy(abcd[i]) << " ";
        outF << precision(abcd[i]) << " ";
        outF << recall(abcd[i]) << " ";
        outF << f1(abcd[i]) << "\n";
    }

    // ------Micro Calculations-------
    outF << overall_accuracy(total_vals) << " ";
    outF << precision(total_vals) << " ";
    outF << recall(total_vals) << " ";
    outF << f1(total_vals) << "\n";

    // ------Macro Calculations-------

    // Needed for macro f1 calculation
    double total_recall = 0;
    double total_precision = 0;

    for(int j = 0; j < stats[0].size() - 1; j++)
    {
        double total_val = 0;

        for(int i = 0; i < stats.size(); i++)
        {
            total_val += stats[i][j];
        }

        outF << (total_val / stats.size()) << " ";

        // To get overall values
        if (j == 1)
            total_precision = (total_val / stats.size());
        if (j == 2)
            total_recall = (total_val / stats.size());
    }
    // f1 (note the function utilizes the B and C values and thus cannot be used here)
    outF << (2 * total_precision * total_recall) / (total_precision + total_recall) << "\n";
}

//Mission control
int main() 
{
    string train_or_test;
    cout << "Please declare whether you would like to train or test your neural network (type either train or test): \n";
    cin >> train_or_test;
    if(train_or_test == "train")
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

        neural_network nn_train(get<0>(input_output), get<0>(w1_w2), get<1>(w1_w2), get<1>(input_output), epochs, lr);
        tuple<vector<vector<double>>, vector<vector<double>>> new_w1_w2 = nn_train.train_network();

        print_trained_weights(output_file, get<0>(new_w1_w2), get<1>(new_w1_w2));
    }
    else if(train_or_test == "test")
    {
        string network_file, testing_data_file, results_file;

        cout << "Please input the name of the already trained neural network file: \n";
        cin >> network_file;

        cout << "Please input the name of the testing data file: \n";
        cin >> testing_data_file;

        cout << "Please input the name of the results file: \n";
        cin >> results_file;

        tuple<vector<vector<double>>, vector<vector<int>>> input_output = read_data(testing_data_file);
        tuple<vector<vector<double>>, vector<vector<double>>> w1_w2 = read_weights(network_file);
        //print_2d(get<1>(input_output));
        //print_2d(get<1>(input_output));

        neural_network nn_test(get<0>(input_output), get<0>(w1_w2), get<1>(w1_w2), get<1>(input_output));
        vector<vector<int>> abcd = nn_test.test_network();

        print_results(results_file, abcd);
    }

    return 0;
}