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
#include <ctime>

using namespace std;

//-1 never allowed a piece
// 0 no piece on legal square
//
bool game::get_whose_move()
{
    return player1_move;
}

vector<vector<tuple<int, int>>> game::get_moves_given_whose_move(bool move, vector<vector<int>> board)
{
    if (move)
        return get_valid_moves_player1(board);
    else
        return get_valid_moves_player2(board);
}



int game::get_computer_move()
{
    clock_t c_start = clock();
    if(get_moves_given_whose_move(player1_move, board_state).size() == 1)
    {
        cout << ((double)(clock() - c_start) / CLOCKS_PER_SEC);
        return 0;
    }

    int i = 1;
    tuple<int, int> heuristic_index = make_tuple(0, 0);
    while(((double)(clock() - c_start) / CLOCKS_PER_SEC) < (double)time_move/2)
    {
        heuristic_index = minimax(board_state, i, player1_move, -100000000, 100000000);
        cout << "Searching to Depth: " << i << "\n";
        cout << "Heuristic Ouput: " << get<0>(heuristic_index) << "\n";
        i++;
    }
    cout << "Time: " << ((double)(clock() - c_start) / CLOCKS_PER_SEC) << "\n";
    return get<1>(heuristic_index);
}


tuple<int, int> game::minimax(vector<vector<int>> board, int depth, bool player1, int alpha, int beta)
{
    vector<vector<tuple<int, int>>> moves = get_moves_given_whose_move(player1, board);
    if (depth == 0 || moves.empty())
    {
        //cout << heuristic_function(board) << "\n";
        return make_tuple(heuristic_function(board),-1);
    }

    if (player1)
    {
        int max_eval = -100000000;
        int index = -1;
        for (int i = 0; i < moves.size(); i++)
        {
            vector<vector<int>> child = make_tmp_move(moves[i], board);
            tuple<int, int> heuristic_index_tmp = minimax(child, depth - 1, false, alpha, beta);
            if (get<0>(heuristic_index_tmp) > max_eval)
            {
                max_eval = get<0>(heuristic_index_tmp);
                index = i;
            }
            else if (get<0>(heuristic_index_tmp) == max_eval)
            {
                if (rand() % 2 == 0)
                {
                    index = i;
                }
            }

            if (alpha < get<0>(heuristic_index_tmp))
                alpha = get<0>(heuristic_index_tmp);

            if (beta <= alpha)
                return make_tuple(max_eval + 10, index);

        }
        return make_tuple(max_eval, index);
    }
    else
    {
        int min_eval = 100000000;
        int index = -1;
        for (int i = 0; i < moves.size(); i++)
        {
            vector<vector<int>> child = make_tmp_move(moves[i], board);
            tuple<int, int> heuristic_index_tmp = minimax(child, depth - 1, true, alpha, beta);
            if (get<0>(heuristic_index_tmp) < min_eval)
            {
                min_eval = get<0>(heuristic_index_tmp);
                index = i;
            }
            else if (get<0>(heuristic_index_tmp) == min_eval)
            {
                if (rand() % 2 == 0)
                {
                    index = i;
                }
            }

            if (beta > get<0>(heuristic_index_tmp))
                beta = get<0>(heuristic_index_tmp);

            if (beta <= alpha)
                return make_tuple(min_eval - 10, index);
        }
        return make_tuple(min_eval, index);

    }
}



int game::heuristic_function(vector<vector<int>> board)
{
    int score = 0;
    int total_white_pieces = 0;
    int total_black_pieces = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int jj = 0; jj < 4; jj++)
        {
            int j = 2*jj;
            if(i % 2 == 0)
                j = 2*jj + 1;

            if (board[i][j] == 1)
            {
                score += 30;
                total_white_pieces++;
            }
                
            else if (board[i][j] == 3)
            {
                score += 50;
                total_white_pieces++;
            }
            else if (board[i][j] == 2)
            {
                score -= 30;
                total_black_pieces++;
            }
            else if (board[i][j] == 4)
            {
                score -= 50;
                total_black_pieces++;
            }
        }
    }
    if (total_black_pieces == 0)
        score += 1000000;
    else if (total_white_pieces == 0)
        score -= 1000000;

    return score + (rand() % 11) - 5;
}

vector<vector<int>> game::make_tmp_move(vector<tuple<int, int>> move_list, vector<vector<int>> board)
{
    for (int i = 0; i < move_list.size()-1; i++)
    {
        int piece = board[get<0>(move_list[i])][get<1>(move_list[i])];

        // Jump
        if (abs(abs(get<0>(move_list[i])) - abs(get<0>(move_list[i+1]))) == 2)
        {
            board[get<0>(move_list[i])][get<1>(move_list[i])] = 0;
            board[(get<0>(move_list[i]) + get<0>(move_list[i+1])) / 2][(get<1>(move_list[i]) + get<1>(move_list[i+1])) / 2] = 0;
        }
        // Non Jump
        else
        {
            board[get<0>(move_list[i])][get<1>(move_list[i])] = 0;
        }
        board[get<0>(move_list[i+1])][get<1>(move_list[i+1])] = piece;

        

        if (piece == 1 && get<0>(move_list[i+1]) == 0 || piece == 2 && get<0>(move_list[i+1]) == 7)
        {
            board[get<0>(move_list[i+1])][get<1>(move_list[i+1])] += 2;
        }
    }

    return board;
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

vector<vector<tuple<int, int>>> game::get_valid_moves_player1(vector<vector<int>> board)
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
                

            if (board[i][j] == 1)
            {
                if (jumps.empty())
                {
                    //find valid non jump moves
                    if (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i - 1, j - 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i - 1 >= 0 && j + 1 <= 7 && board[i - 1][j + 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i - 1, j + 1}};
                        regular.push_back(tmpVect);
                    }
                }

                vector<tuple<int, int>> tmpVect = {{i, j}};
                jumps = get_valid_jumps_player1(board, i, j, board[i][j], jumps, tmpVect);
            }
            else if (board[i][j] == 3)
            {
                if (jumps.empty())
                {
                    //find valid non jump moves
                    if (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i - 1, j - 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i - 1 >= 0 && j + 1 <= 7 && board[i - 1][j + 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i - 1, j + 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i + 1 <= 7 && j + 1 <= 7 && board[i + 1][j + 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i + 1, j + 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i + 1 <= 7 && j - 1 >= 0 && board[i + 1][j - 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i + 1, j - 1}};
                        regular.push_back(tmpVect);
                    }

                }

                //find valid jumps moves for kings always
                vector<tuple<int, int>> tmpVect = {{i, j}};
                jumps = get_valid_jumps_player1(board, i, j, board[i][j], jumps, tmpVect);
            }
        };
    };

    if (jumps.empty()) {
        return regular;
    }

    return jumps;
};

vector<vector<tuple<int, int>>> game::get_valid_moves_player2(vector<vector<int>> board)
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

            if (board[i][j] == 2)
            {
                if (jumps.empty())
                {
                    //find valid non jump moves
                    if (i + 1 >= 0 && j - 1 >= 0 && board[i + 1][j - 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i + 1, j - 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i + 1 >= 0 && j + 1 <= 7 && board[i + 1][j + 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i + 1, j + 1}};
                        regular.push_back(tmpVect);
                    }
                }

                vector<tuple<int, int>> tmpVect = {{i, j}};
                jumps = get_valid_jumps_player2(board, i, j, board[i][j], jumps, tmpVect);
            }
            else if (board[i][j] == 4)
            {
                if (jumps.empty())
                {
                    //find valid non jump moves
                    if (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i - 1, j - 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i - 1 >= 0 && j + 1 <= 7 && board[i - 1][j + 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i - 1, j + 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i + 1 <= 7 && j + 1 <= 7 && board[i + 1][j + 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i + 1, j + 1}};
                        regular.push_back(tmpVect);
                    }
                    if (i + 1 <= 7 && j - 1 >= 0 && board[i + 1][j - 1] == 0) {
                        vector<tuple<int, int>> tmpVect = {{i, j}, {i + 1, j - 1}};
                        regular.push_back(tmpVect);
                    }

                }

                //find valid jumps moves for kings always
                vector<tuple<int, int>> tmpVect = {{i, j}};
                jumps = get_valid_jumps_player2(board, i, j, board[i][j], jumps, tmpVect);
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



