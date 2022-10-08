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

// //Confirms valid word before sending it to be spellchecked
// void wordConfirmation(string word, int line, bool hasNum, ofstream &outF, hashTable *dictionaryHash)
// {
//     if (hasNum == false) //Number check
//     {
//         if(word.length() > 20)  //Length Check
//         {
//             outF << "Long word at line " << line << ", starts: " << word.substr(0,20) << "\n";
//         }
//         else
//         {
//             if (dictionaryHash->contains(word) == false) //See if the word is in the dictionary
//             {
//                 outF << "Unknown word at line " << line << ": " << word << "\n";
//             }
//         }
//     }        
// }

void print_valid_moves(vector<vector<tuple<int, int>>> moves) 
{
    for (int i = 0; i < moves.size(); i++)
    {
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
    bool black_move = false;
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
                black_move = true;            
        }
        else if (row == 9 )
        {
            time = stoi(oneLine);
        }
        
        row++;
    }

    return make_tuple(tmp, black_move, time);

}

//Mission control
int main() 
{
    // vector<vector<int>> i = {0};
    //Ask user for dictionary
    string custom_board, file;


    cout << "Would you like to start with a custom board position? (Type y for yes)\n";
    cin >> custom_board;

    if (custom_board == "y")
    {
        cout << "What file would you like to import from?\n";
        cin >> file;
        vector<vector<int>> board;
        bool black_move;
        int time;

        tie(board, black_move, time) = parse_file(file);
        game g(board, black_move, time);
        g.print_board();
        vector<vector<tuple<int, int>>> moves = g.get_valid_moves_black();
        print_valid_moves(moves);
    }
    else 
    {
        game g;
        g.print_board();
        vector<vector<tuple<int, int>>> moves = g.get_valid_moves_black();
        print_valid_moves(moves);
    }
    


    // //Call function to spellcheck the input file's words and record the time
    // c_start = clock();
    // parseText(inputFile, outputFile, dictionaryHash);
    // c_end = clock();
    // time_elapsed_s = ((double)(c_end-c_start)) / CLOCKS_PER_SEC;
    // cout << "Time Elapsed to Parse Input: " << time_elapsed_s << " s\n";
    
    return 0;
}