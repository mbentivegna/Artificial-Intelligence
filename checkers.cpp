/*
Michael Bentivegna
Artificial Intelligence 
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
        cout << (i + 1) << ".  ";
        for (int j = 0; j < moves[i].size(); j++)
        {
            cout << "(" << (1 + get<1>(moves[i][j])) << "," << (8 - get<0>(moves[i][j])) << ")";
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

void print_winner(bool player1_loser)
{
    cout << "   There are no valid moves :(\n\n";

    if (!player1_loser)
        cout << "\u001b[38;5;1m" << "\n\n\nPLAYER 1 HAS WON CONGRATS!!!\n\n\n" << "\u001b[0m";
    else
        cout << "\u001b[38;5;21m" << "\n\n\nPLAYER 2 HAS WON CONGRATS!!!\n\n\n" << "\u001b[0m";

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

    // x axis +1
    // y axis 8 - i
    // (x,y)

    while(true) 
    {
        bool whose_move = g.get_whose_move();
        vector<vector<tuple<int, int>>> moves = g.get_moves_given_whose_move(whose_move);
        g.print_board();
        if (moves.empty())
        {
            print_winner(whose_move);
            break;
        }
        else 
        {
            string chosen_move;
            print_valid_moves(moves);
            cout << "\n\nPlease select a move to make: \n";
            cin >> chosen_move;
            g.make_move(moves[stoi(chosen_move) - 1]);
        }

    }




    


    // //Call function to spellcheck the input file's words and record the time
    // c_start = clock();
    // parseText(inputFile, outputFile, dictionaryHash);
    // c_end = clock();
    // time_elapsed_s = ((double)(c_end-c_start)) / CLOCKS_PER_SEC;
    // cout << "Time Elapsed to Parse Input: " << time_elapsed_s << " s\n";
    
    return 0;
}