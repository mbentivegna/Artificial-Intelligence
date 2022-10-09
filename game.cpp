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

game::game(vector<vector<int>> board, bool move, int time)
{   
    if (!board.empty())
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                board_state[i][j] = board[i][j];
            };
        };
    }

    player1_move = move;
    time_move = time;
}

vector<vector<tuple<int, int>>> game::get_valid_jumps_player1(vector<vector<int>> board, int i, int j, int piece_val,  vector<vector<tuple<int, int>>> current_jumps, vector<tuple<int, int>> tmpVect)
{
    int checker = 0; 
    //find valid jump moves for non-kings always
    if (i - 2 >= 0 && j - 2 >= 0 && (board[i - 1][j - 1] == 2 || board[i - 1][j - 1] == 4) && board[i - 2][j - 2] == 0) 
    {
        checker++;
        vector<tuple<int, int>> tmpVector = tmpVect;
        tmpVector.push_back(make_tuple(i-2, j-2));
        vector<vector<int>> tmp_board = board;
        tmp_board[i][j] = 0;
        tmp_board[i-1][j-1] = 0;
        tmp_board[i-2][j-2] = piece_val;
        current_jumps = get_valid_jumps_player1(tmp_board, i - 2, j - 2, piece_val, current_jumps, tmpVector);
    }
    if (i - 2 >= 0 && j + 2 <= 7 && (board[i - 1][j + 1] == 2 || board[i - 1][j + 1] == 4) && board[i - 2][j + 2] == 0) 
    {
        checker++;
        vector<tuple<int, int>> tmpVector = tmpVect;
        tmpVector.push_back(make_tuple(i-2, j+2));
        vector<vector<int>> tmp_board = board;
        tmp_board[i][j] = 0;
        tmp_board[i-1][j+1] = 0;
        tmp_board[i-2][j+2] = piece_val;
        current_jumps = get_valid_jumps_player1(tmp_board, i - 2, j + 2, piece_val, current_jumps, tmpVector);
    }
    if (piece_val == 3 && i + 2 <= 7 && j + 2 <= 7 && (board[i + 1][j + 1] == 2 || board[i + 1][j + 1] == 4) && board[i + 2][j + 2] == 0) 
    {
        checker++;
        vector<tuple<int, int>> tmpVector = tmpVect;
        tmpVector.push_back(make_tuple(i+2, j+2));
        vector<vector<int>> tmp_board = board;
        tmp_board[i][j] = 0;
        tmp_board[i+1][j+1] = 0;
        tmp_board[i+2][j+2] = piece_val;
        current_jumps = get_valid_jumps_player1(tmp_board, i + 2, j + 2, piece_val, current_jumps, tmpVector);
    }
    if (piece_val == 3 && i + 2 <= 7 && j - 2 >= 0 && (board[i + 1][j - 1] == 2 || board[i + 1][j - 1] == 4) && board[i + 2][j - 2] == 0) 
    {
        checker++;
        vector<tuple<int, int>> tmpVector = tmpVect;
        tmpVector.push_back(make_tuple(i+2, j-2));
        vector<vector<int>> tmp_board = board;
        tmp_board[i][j] = 0;
        tmp_board[i+1][j-1] = 0;
        tmp_board[i+2][j-2] = piece_val;
        current_jumps = get_valid_jumps_player1(tmp_board, i + 2, j - 2, piece_val, current_jumps, tmpVector);
    }

    if (tmpVect.size() != 1 && checker == 0)
        current_jumps.push_back(tmpVect);

    return current_jumps;
    
}

vector<vector<tuple<int, int>>> game::get_valid_jumps_player2(vector<vector<int>> board, int i, int j, int piece_val,  vector<vector<tuple<int, int>>> current_jumps, vector<tuple<int, int>> tmpVect)
{
    int checker = 0; 
    //find valid jump moves for non-kings always
    if (piece_val == 4 && i - 2 >= 0 && j - 2 >= 0 && (board[i - 1][j - 1] == 1 || board[i - 1][j - 1] == 3) && board[i - 2][j - 2] == 0) 
    {
        checker++;
        vector<tuple<int, int>> tmpVector = tmpVect;
        tmpVector.push_back(make_tuple(i-2, j-2));
        vector<vector<int>> tmp_board = board;
        tmp_board[i][j] = 0;
        tmp_board[i-1][j-1] = 0;
        tmp_board[i-2][j-2] = piece_val;
        current_jumps = get_valid_jumps_player2(tmp_board, i - 2, j - 2, piece_val, current_jumps, tmpVector);
    }
    if (piece_val == 4 && i - 2 >= 0 && j + 2 <= 7 && (board[i - 1][j + 1] == 1 || board[i - 1][j + 1] == 3) && board[i - 2][j + 2] == 0) 
    {
        checker++;
        vector<tuple<int, int>> tmpVector = tmpVect;
        tmpVector.push_back(make_tuple(i-2, j+2));
        vector<vector<int>> tmp_board = board;
        tmp_board[i][j] = 0;
        tmp_board[i-1][j+1] = 0;
        tmp_board[i-2][j+2] = piece_val;
        current_jumps = get_valid_jumps_player2(tmp_board, i - 2, j + 2, piece_val, current_jumps, tmpVector);
    }
    if (i + 2 <= 7 && j + 2 <= 7 && (board[i + 1][j + 1] == 1 || board[i + 1][j + 1] == 3) && board[i + 2][j + 2] == 0) 
    {
        checker++;
        vector<tuple<int, int>> tmpVector = tmpVect;
        tmpVector.push_back(make_tuple(i+2, j+2));
        vector<vector<int>> tmp_board = board;
        tmp_board[i][j] = 0;
        tmp_board[i+1][j+1] = 0;
        tmp_board[i+2][j+2] = piece_val;
        current_jumps = get_valid_jumps_player2(tmp_board, i + 2, j + 2, piece_val, current_jumps, tmpVector);
    }
    if (i + 2 <= 7 && j - 2 >= 0 && (board[i + 1][j - 1] == 1 || board[i + 1][j - 1] == 3) && board[i + 2][j - 2] == 0) 
    {
        checker++;
        vector<tuple<int, int>> tmpVector = tmpVect;
        tmpVector.push_back(make_tuple(i+2, j-2));
        vector<vector<int>> tmp_board = board;
        tmp_board[i][j] = 0;
        tmp_board[i+1][j-1] = 0;
        tmp_board[i+2][j-2] = piece_val;
        current_jumps = get_valid_jumps_player2(tmp_board, i + 2, j - 2, piece_val, current_jumps, tmpVector);
    }

    if (tmpVect.size() != 1 && checker == 0)
        current_jumps.push_back(tmpVect);

    return current_jumps;
    
}

vector<vector<tuple<int, int>>> game::get_valid_moves_player1()
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

                vector<tuple<int, int>> tmpVect = {{i, j}};
                jumps = get_valid_jumps_player1(board_state, i, j, board_state[i][j], jumps, tmpVect);
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
                vector<tuple<int, int>> tmpVect = {{i, j}};
                jumps = get_valid_jumps_player1(board_state, i, j, board_state[i][j], jumps, tmpVect);
            }
        };
    };

    if (jumps.empty()) {
        return regular;
    }

    return jumps;
};

vector<vector<tuple<int, int>>> game::get_valid_moves_player2()
{
    vector<vector<tuple<int, int>>> jumps;
    vector<vector<tuple<int, int>>> regular;
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board_state[i][j] == 2)
            {
                if (jumps.empty())
                {
                    //find valid non jump moves
                    if (i + 1 >= 0 && j - 1 >= 0 && board_state[i + 1][j - 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i + 1, j - 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i + 1 >= 0 && j + 1 <= 7 && board_state[i + 1][j + 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i + 1, j + 1}};
                        regular.push_back(tmpVect);
                    }
                }

                vector<tuple<int, int>> tmpVect = {{i, j}};
                jumps = get_valid_jumps_player2(board_state, i, j, board_state[i][j], jumps, tmpVect);
            }
            else if (board_state[i][j] == 4)
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
                vector<tuple<int, int>> tmpVect = {{i, j}};
                jumps = get_valid_jumps_player2(board_state, i, j, board_state[i][j], jumps, tmpVect);
            }
        };
    };

    if (jumps.empty()) {
        return regular;
    }

    return jumps;
};

void game::print_board()
{
    cout << "              PLAYER 2 \n\n";
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
    cout << "\n              PLAYER 1 \n ";
    if(player1_move)
        cout << "\nIt is Player 1's move (O)\n";
    else
        cout << "\nIt is Player 2's move (X)\n";

    // cout << "\n" << time_move << "\n";
}



