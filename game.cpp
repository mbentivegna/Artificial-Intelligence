/*
Michael Bentivegna
Artificial Intelligence 
*/

#include "game.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <thread>
#include <chrono>

using namespace std;

//-1 never allowed a piece
// 0 no piece on legal square
//
bool game::get_whose_move()
{
    return player1_move;
}

vector<vector<tuple<int, int>>> game::get_moves_given_whose_move(bool move)
{
    if (move)
        return get_valid_moves_player1();
    else
        return get_valid_moves_player2();
}

int game::get_computer_move()
{
    this_thread::sleep_for(3000ms);
    int length = get_moves_given_whose_move(player1_move).size();

    return rand() % length;
}


void game::make_move(vector<tuple<int, int>> move_list)
{
    for (int i = 0; i < move_list.size()-1; i++)
    {
        int piece = board_state[get<0>(move_list[i])][get<1>(move_list[i])];

        // Jump
        if (abs(abs(get<0>(move_list[i])) - abs(get<0>(move_list[i+1]))) == 2)
        {
            board_state[get<0>(move_list[i])][get<1>(move_list[i])] = 0;
            board_state[(get<0>(move_list[i]) + get<0>(move_list[i+1])) / 2][(get<1>(move_list[i]) + get<1>(move_list[i+1])) / 2] = 0;
        }
        // Non Jump
        else
        {
            board_state[get<0>(move_list[i])][get<1>(move_list[i])] = 0;
        }
        board_state[get<0>(move_list[i+1])][get<1>(move_list[i+1])] = piece;

        

        if (piece == 1 && get<0>(move_list[i+1]) == 0 || piece == 2 && get<0>(move_list[i+1]) == 7)
        {
            board_state[get<0>(move_list[i+1])][get<1>(move_list[i+1])] += 2;
        }
    }

    player1_move = !player1_move;
}

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
        for (int jj = 0; jj < 4; jj++)
        {
            int j = 2*jj;
            if(i % 2 == 0)
                j = 2*jj + 1;
                

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
        for (int jj = 0; jj < 4; jj++)
        {
            int j = 2*jj;
            if(i % 2 == 0)
                j = 2*jj + 1;

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

vector<vector<int>> game::print_board()
{
    cout << "                      " << "\u001b[38;5;21m" << "PLAYER 2\n\n" << "\u001b[0m";
    for (int i = 0; i < 8; i++)
    {
        
        for (int k = 0; k < 3; k++)
        {
            if (k == 1)
                cout << " " << (8 - i) << "  ";
            else
                cout << "    ";
            for (int j = 0; j < 8; j++)
            {
                int tmp = board_state[i][j];
                if (tmp == -1)
                    cout <<  "\u001b[48;5;140m" <<"      " << "\u001b[0m";
                else if (tmp == 0 && k == 1)
                    cout << "      ";
                else if (tmp == 1 && k == 1) 
                    cout << "  " << "\u001b[48;5;1m" <<"  " << "\u001b[0m" << "  ";
                else if (tmp == 2 && k == 1)
                    cout << "  " << "\u001b[48;5;21m" <<"  " << "\u001b[0m" << "  ";
                else if (tmp == 3 && k == 1)
                    cout << "  " << "\u001b[48;5;1m" <<"##" << "\u001b[0m" << "  ";
                else if (tmp == 4 && k == 1)
                    cout << "  " << "\u001b[48;5;21m" <<"##" << "\u001b[0m" << "  ";
                else
                    cout << "      ";
            };
            cout << "\n";
        };
        
    };
    cout << "\n      1     2     3     4     5     6     7     8 \n\n";




    cout << "                      " << "\u001b[38;5;1m" << "PLAYER 1 \n" << "\u001b[0m";
    if(player1_move)
        cout << "\nIt is " << "\u001b[38;5;1m" << "PLAYER 1" << "\u001b[0m" << "'s move\n\n";
    else
        cout << "\nIt is " << "\u001b[38;5;21m" << "PLAYER 2" << "\u001b[0m" << "'s move\n\n";

    // cout << "\n" << time_move << "\n";

    return board_state;
}



