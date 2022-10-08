/*
Michael Bentivegna
Hashtable Spellcheck Assignment

    A summary of this project can be found at the top of spellcheck.cpp.  This file implements the hashtable that is used to load the dictionary.
    It has the ability to rehash based on the number of words the dictionary contains and uses linear probing for collisions.
*/

#include "game.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

//-1 never allowed a piece
// 0 no piece on legal square
//

game::game()
{
    black_move = true;
    time_move = 5;
}

game::game(vector<vector<int>> vect, bool move, int time)
{   
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board_state[i][j] = vect[i][j];
        };
    };

    black_move = move;
    time_move = time;
}




vector<vector<tuple<int, int>>> game::get_valid_moves_black()
{
    vector<vector<tuple<int, int>>> jumps;
    vector<vector<tuple<int, int>>> regular;
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board_state[i][j] == 1)
            {
                if (jumps.empty())
                {
                    //find valid non jump moves
                    if (i - 1 >= 0 && j - 1 >= 0 && board_state[i - 1][j - 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i - 1, j - 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i - 1 >= 0 && j + 1 <= 7 && board_state[i - 1][j + 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i - 1, j + 1}};
                        regular.push_back(tmpVect);
                    }
                }
                
                //find valid jump moves for non-kings always


            }
            else if (board_state[i][j] == 3)
            {
                if (jumps.empty())
                {
                    //find valid non jump moves
                    if (i - 1 >= 0 && j - 1 >= 0 && board_state[i - 1][j - 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i - 1, j - 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i - 1 >= 0 && j + 1 <= 7 && board_state[i - 1][j + 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i - 1, j + 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i + 1 <= 7 && j + 1 <= 7 && board_state[i + 1][j + 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i + 1, j + 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i + 1 <= 7 && j - 1 >= 0 && board_state[i + 1][j - 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i + 1, j - 1}};
                        regular.push_back(tmpVect);
                    }

                }

                //find valid jumps moves for kings always
            }
        };
    };

    if (jumps.empty()) {
        return regular;
    }

    return jumps;
    //print for now


    

}

void game::print_board()
{
    cout << "     0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 \n";
    cout << "    -------------------------------\n";
    for (int i = 0; i < 8; i++)
    {
        cout << i << "  |";
        for (int j = 0; j < 8; j++)
        {
            int tmp = board_state[i][j];
            if (tmp == -1)
                cout << "   |";
            else if (tmp == 0)
                cout << "   |";
            else if (tmp == 1) 
                cout << " O |";
            else if (tmp == 2)
                cout << " X |";
            else if (tmp == 3)
                cout << " KO|";
            else if (tmp == 4)
                cout << " KX|";
        };
        cout << "\n    -------------------------------\n";
    };
    if(black_move)
        cout << "\nIt is Black's move (O)\n";
    else
        cout << "\nIt is White's move (X)\n";

    cout << "\n" << time_move << "\n";
}

// //Returns the first prime number bigger than size in the above sequence
// unsigned int hashTable::getPrime(int size) {

//     for (int i = 0; i < primes.size(); i++) 
//     {
//             if(primes[i] >= size)
//             {
//                 return primes[i];
//             }
//     }

//     return 0;
// }


