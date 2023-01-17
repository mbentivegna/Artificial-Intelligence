/*
Michael Bentivegna
Artificial Intelligence 
Neural Network

******** ONLY USED TO GENERATE INITIAL WEIGHTS FOR MY DATASET *********
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

int main()
{
    srand((unsigned)time(NULL));

    ofstream outF;
    outF.open("my_dataset/initial_weights.txt");

    // Layer sizes
    int input_layer = 4;
    int hidden_layer = 20;
    int output_layer = 1;

    // Print first line about layers
    outF << input_layer << " " << hidden_layer << " " << output_layer << "\n";
    outF << setprecision(3) << fixed;

    // Print randomized w1
    for (int i = 0; i < hidden_layer; i++)
    {
        // Need +1 for the bias
        for(int j = 0; j < input_layer + 1; j++)
        {
            if (j != 0)
                outF << " ";
            outF << (double)rand() / (double)RAND_MAX;
        }
        outF << "\n";
    }

    // Print randomized w2
    for (int i = 0; i < output_layer; i++)
    {
        // Need +1 for the bias
        for(int j = 0; j < hidden_layer + 1; j++)
        {
            if (j != 0)
                outF << " ";
            outF << (double)rand() / (double)RAND_MAX;
        }
        outF << "\n";
    }

    return 0;
}