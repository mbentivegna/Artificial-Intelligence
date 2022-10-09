/*
    Michael Bentivegna
    Spellcheck Assignment

    This project initially loads a dictionary into a hashtable.  It then takes in a user selected input file to spell check it with the allotted dictionary.
    If the word is either too long or contains a number it is omitted from the spell check process.  The proper output is then written to a user specified file.
*/


#include "game.h"
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

void print_valid_moves(vector<vector<tuple<int, int>>> moves) 
{
    for (int i = 0; i < moves.size(); i++)
    {
        cout << i << ".  ";
        for (int j = 0; j < moves[i].size(); j++)
        {
            cout << "(" << get<0>(moves[i][j]) << "," << get<1>(moves[i][j]) << ")";
            if (j != moves[i].size() - 1)
                cout << " --> ";
            
        }
        cout << "\n";
    }
}

tuple<vector<vector<int>>, bool, int> parse_file(string file){

    ifstream inF;
    inF.open(file);
    vector<vector<int>> tmp(8, vector<int> (8, -1));
    bool player1_move = false;
    int time = 0;

    string oneLine;
    int row = 0;
    while (getline(inF, oneLine) && row < 10)
    {
        
        if (row < 8)
        {
            istringstream iss(oneLine);
            string word;
            int column = 0;
            while (iss >> word) {  
            if (row % 2 == 0)
                tmp[row][2*column+1] = stoi(word);
            else
                tmp[row][2*column] = stoi(word);

            column++;
            };

            
        }
        else if (row == 8)
        {
            if (stoi(oneLine) == 1)
                player1_move = true;            
        }
        else if (row == 9 )
        {
            time = stoi(oneLine);
        }
        
        row++;
    }

    return make_tuple(tmp, player1_move, time);

}

//Mission control
int main() 
{
    // vector<vector<int>> i = {0};
    //Ask user for dictionary
    string custom_board, file, runtime, move_first;

    cout << "Would you like to start with a custom board position from a txt file? (Type y for yes)\n";
    cin >> custom_board;

    vector<vector<int>> board;
    bool player1_move;
    int time;

    if (custom_board == "y")
    {
        cout << "What file would you like to import from?\n";
        cin >> file;
        

        tie(board, player1_move, time) = parse_file(file);
        
    }

    else 
    {
        cout << "Please enter the computer runtime limit (in seconds):\n";
        cin >> runtime;

        cout << "Please enter who should move first (Type 1 or 2)\n";
        cin >> move_first;
        player1_move = false;
        if (stoi(move_first) == 1)
            player1_move = true;
        time = stoi(runtime);
    }
    
    game g(board, player1_move, time);

    g.print_board();
    if (player1_move)
        print_valid_moves(g.get_valid_moves_player1());
    else
        print_valid_moves(g.get_valid_moves_player2());


    


    // //Call function to spellcheck the input file's words and record the time
    // c_start = clock();
    // parseText(inputFile, outputFile, dictionaryHash);
    // c_end = clock();
    // time_elapsed_s = ((double)(c_end-c_start)) / CLOCKS_PER_SEC;
    // cout << "Time Elapsed to Parse Input: " << time_elapsed_s << " s\n";
    
    return 0;
}