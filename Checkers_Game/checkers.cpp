/*
Michael Bentivegna
Artificial Intelligence 
Checkers
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

// Print a list of all valid moves in easy to read format for the user
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

// Parse input file for board state, whose move it is, and how much time the computer is given
tuple<vector<vector<int>>, bool, double> parse_file(string file){

    ifstream inF;
    inF.open(file);
    vector<vector<int>> tmp(8, vector<int> (8, -1));
    bool player1_move = false;
    double time = 0;

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
            time = stod(oneLine);
        }
        
        row++;
    }

    return make_tuple(tmp, player1_move, time);

}

// Print the winner in their proper color
void print_winner(bool player1_loser, bool resigned)
{
    if (!resigned)
        cout << "   There are no valid moves :(\n\n";

    if (!player1_loser)
        cout << "\u001b[38;5;1m" << "\n\n\nPLAYER 1 HAS WON CONGRATS!!!\n\n\n" << "\u001b[0m";
    else
        cout << "\u001b[38;5;21m" << "\n\n\nPLAYER 2 HAS WON CONGRATS!!!\n\n\n" << "\u001b[0m";

}

//Mission control
int main() 
{
    // Random number generator seeding
    srand(time(NULL));

    //Get information about the board
    string custom_board, file, move_first;

    string player1_comp_str, player2_comp_str;
    cout << "Would you like player 1 to be a computer? (Type y for yes)\n";
    cin >> player1_comp_str;

    cout << "Would you like player 2 to be a computer? (Type y for yes)\n";
    cin >> player2_comp_str;


    cout << "Would you like to start with a custom board position from a txt file? (Type y for yes)\n";
    cin >> custom_board;

    vector<vector<int>> board;
    bool player1_move;
    double time;

    if (custom_board == "y")
    {
        cout << "What file would you like to import from?\n";
        cin >> file;
        

        tie(board, player1_move, time) = parse_file(file);
        
    }

    else 
    {
        cout << "Please enter the computer runtime limit (in seconds):\n";
        cin >> time;
        while(true)
        {
            if (!cin)
            {
                cout << "You input an invalid runtime. Please try again: \n";
            }
            else
            {
                break;
            }
            cin.clear();
            cin.ignore(1000,'\n');
            cin >> time;
        }

        cout << "Please enter who should move first (Type 1 or 2)\n";
        cin >> move_first;
        while(true)
        {
            if(move_first == "1")
            {
                player1_move = true;
                break;
            }
            else if (move_first == "2")
            {
                player1_move = false;
                break;
            }
            else
            {
                cout << "Invalid Starting User... Please enter who should move first (Type 1 or 2)\n";
                cin >> move_first;
            }
        }
    }
    
    // Initialize game object
    game g(board, player1_move, time);

    // Make moves until game ends
    while(true) 
    {
        bool whose_move = g.player1_move;
        vector<vector<tuple<int, int>>> moves = g.get_moves_given_whose_move(whose_move, g.board_state);
        g.print_board();

        if (moves.empty())
        {
            print_winner(whose_move, false);
            break;
        }
        else 
        {
            if (player1_comp_str == "y" && whose_move || player2_comp_str == "y" && !whose_move)
            {
                //print_valid_moves(moves);
                //cout << g.draw_checker;
                cout << "-----WAITING FOR COMPUTER-----\n\n";
                int chosen = g.get_computer_move();
                g.make_move(moves[chosen]);
            }
            else
            {
                print_valid_moves(moves);
                int chosen_move;
                int num_valid_moves = moves.size();
                cout << "\n\nPlease select a move to make: \n";
                cin >> chosen_move;
                while(true)
                {
                    if (!cin || chosen_move < 1 || chosen_move > num_valid_moves)
                    {
                        if (chosen_move == -1)
                        {
                            string resign;
                            cout << "Are you sure you want to resign the game? (Type y to resign)\n";
                            cin >> resign;
                            if (resign == "y")
                            {
                                cout << "THE GAME HAS BEEN RESIGNED\n";
                                print_winner(whose_move, true);
                                return 0;
                            }
                            else
                                cout << "You have chosen not to resign. Please input a valid move:\n";

                        }
                        else
                            cout << "You input an invalid move. Please try again: \n";
                    }
                    else
                    {
                        break;
                    }
                    cin.clear();
                    cin.ignore(1000,'\n');
                    cin >> chosen_move;
                }
                g.make_move(moves[chosen_move - 1]);
            }
        }
    }
    
    return 0;
}