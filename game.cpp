/*
Michael Bentivegna
Artificial Intelligence 
Checkers

*/

#include "game.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <thread>
#include <chrono>
#include <ctime>
#include <algorithm>

using namespace std;

vector<vector<tuple<int, int>>> game::get_moves_given_whose_move(bool move, vector<vector<int>> board)
{
    if (move)
        return get_valid_moves_player1(board);
    else
        return get_valid_moves_player2(board);
}

bool game::two_on_one_endgame()
{
    int total_pieces = 0;
    int p1_kings = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int jj = 0; jj < 4; jj++)
        {
            int j = 2*jj;
            if(i % 2 == 0)
                j = 2*jj + 1;

            if (board_state[i][j] == 1)
            {
                return false;
            }
                
            else if (board_state[i][j] == 3)
            {
                total_pieces++;
                p1_kings++;
            }
            else if (board_state[i][j] == 2)
            {
                return false;
            }
            else if (board_state[i][j] == 4)
            {
                total_pieces++;
            }
        }
    }

    if (total_pieces == 3)
    {
        if ((p1_kings == 2 && player1_move) || (p1_kings == 1 && !player1_move))
        {
            return true;
        }
    }
    
    return false;
}

int game::get_computer_move()
{
    clock_t c_start = clock();
    if(get_moves_given_whose_move(player1_move, board_state).size() == 1)
    {
        cout << "Time: " << ((double)(clock() - c_start) / CLOCKS_PER_SEC) << "\n";
        cout << "Depth Searched: 0" << "\n\n\n";
        return 0;
    }

    int i = 1;

    tuple<int, int, bool> heuristic_index = make_tuple(0, 0, false);
    bool two_on_one = two_on_one_endgame();
    vector<int> list_moves;
    while(true)
    {
        tuple<int, int, bool> tmp = minimax(board_state, i, player1_move, -100000000, 100000000, c_start);
        // cout << get<0>(tmp) << "  " << get<1>(tmp) << "  " << get<2>(tmp) << "\n";
        if(get<2>(tmp))
        {
            break;
        }
        else
        {
            heuristic_index = tmp;
            list_moves.push_back(get<1>(heuristic_index));
            cout << "Heuristic: " << get<0>(heuristic_index) << "\n";
            cout << "Move: " << get<1>(heuristic_index) << "\n";
        }
        i++;

        if (abs(get<0>(heuristic_index)) > 900000)
            break;

    }

    cout << "Time: " << ((double)(clock() - c_start) / CLOCKS_PER_SEC) << "\n";
    cout << "Depth Searched: " << i - 1 << "\n\n\n";

    //return mode in special 2 on 1 case
    if (two_on_one && i > 10 && abs(get<0>(heuristic_index)) < 900000)
    {
        sort(list_moves.begin(), list_moves.end());

        int val = list_moves[0];
        int mode = val;
        int count = 1;
        int countMode = 1;

        for(int i = 1; i < list_moves.size(); i++)
        {
            if (list_moves[i] == val)
            {
                count++;

                if (count > countMode)
                {
                    countMode = count;
                    mode = val;
                }
            }
            else
            {
                val = list_moves[i];
                count = 1;
            }
        }
        return mode;
    }

    return get<1>(heuristic_index);
}

tuple<int, int, bool> game::minimax(vector<vector<int>> board, int depth, bool player1, int alpha, int beta, clock_t start_time)
{
    if (((double)(clock() - start_time) / CLOCKS_PER_SEC) > time_move - .05)
    {
        return make_tuple(-1, -1, true);
    }

    vector<vector<tuple<int, int>>> moves = get_moves_given_whose_move(player1, board);
    
    if (depth == 0 || moves.empty())
    {
        //cout << heuristic_function(board) << "\n";
        return make_tuple(heuristic_function(board, depth), -1, false);
    }

    if (player1)
    {
        int max_eval = -100000000;
        int index = -1;
        for (int i = 0; i < moves.size(); i++)
        {           
            vector<vector<int>> child = make_tmp_move(moves[i], board);
            tuple<int, int, bool> heuristic_index_tmp = minimax(child, depth - 1, false, alpha, beta, start_time);
            // if(firstTime)
            //         cout << "  " << get<0>(heuristic_index_tmp) << "\n";
            if (get<0>(heuristic_index_tmp) > max_eval)
            {
                max_eval = get<0>(heuristic_index_tmp);
                index = i;
            }
            else if (get<0>(heuristic_index_tmp) == max_eval)
            {
                int test = rand() % (moves.size() - 1);
                if (test == 0)
                {
                    index = i;
                }
            }

            if (alpha < get<0>(heuristic_index_tmp))
                alpha = get<0>(heuristic_index_tmp);

            if (beta <= alpha)
            {
                return make_tuple(max_eval + 1, index, false);
            }

        }
        if (((double)(clock() - start_time) / CLOCKS_PER_SEC) > time_move - .05)
        {
            return make_tuple(-1, -1, true);
        }
        return make_tuple(max_eval, index, false);
    }
    else
    {
        int min_eval = 100000000;
        int index = -1;
        for (int i = 0; i < moves.size(); i++)
        {
            vector<vector<int>> child = make_tmp_move(moves[i], board);
            tuple<int, int, bool> heuristic_index_tmp = minimax(child, depth - 1, true, alpha, beta, start_time);
            // if(firstTime)
            //         cout << "  " << get<0>(heuristic_index_tmp) << "\n";
            if (get<0>(heuristic_index_tmp) < min_eval)
            {
                min_eval = get<0>(heuristic_index_tmp);
                index = i;
            }
            else if (get<0>(heuristic_index_tmp) == min_eval)
            {
                int test = rand() % (moves.size() / 2);
                if (test == 0)
                {
                    index = i;
                }
            }

            if (beta > get<0>(heuristic_index_tmp))
                beta = get<0>(heuristic_index_tmp);

            if (beta <= alpha)
            {
                return make_tuple(min_eval - 1, index, false);
            }
        }
        if (((double)(clock() - start_time) / CLOCKS_PER_SEC) > time_move - .05)
        {
            return make_tuple(-1, -1, true);
        }
        return make_tuple(min_eval, index, false);

    }
    
}

int game::heuristic_function(vector<vector<int>> board, int depth)
{
    int score = 0;
    int total_white_pieces = 0;
    int total_black_pieces = 0;
    int whose_winning = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int jj = 0; jj < 4; jj++)
        {
            int j = 2*jj;
            if(i % 2 == 0)
                j = 2*jj + 1;

            if (board[i][j] == 1)
            {
                score += 300;
                total_white_pieces++;
            }
                
            else if (board[i][j] == 3)
            {
                score += 500;
                total_white_pieces++;
            }
            else if (board[i][j] == 2)
            {
                score -= 300;
                total_black_pieces++;
            }
            else if (board[i][j] == 4)
            {
                score -= 500;
                total_black_pieces++;
            }
        }
    }
    
    whose_winning = score;

    //Endgames
    if (total_white_pieces + total_black_pieces < 12)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int jj = 0; jj < 4; jj++)
            {
                int j = 2*jj;
                if(i % 2 == 0)
                    j = 2*jj + 1;

                if (board[i][j] == 1)
                {
                    score += (8 - i) * 10;
                }        
                else if (board[i][j] == 3)
                {
                    score += 100;
                }
                else if (board[i][j] == 2)
                {
                    score -= i * 10;
                }
                else if (board[i][j] == 4)
                {
                    score -= 100;

                }
            }
        }    

    // 1 if white winning, 2 if black winning in the current board state
    // See if you are winning in the end game if so then trade if possible
        if(whose_winning >= 200)
        {
            score -= 10 * (total_black_pieces + total_white_pieces);
            if(total_black_pieces + total_white_pieces < 6)
            {
                if(board[1][0] == 4)
                    score -= 30;
                if(board[0][1] == 4)
                    score -= 30;
                if(board[7][6] == 4)
                    score -= 30;
                if(board[6][7] == 4)
                    score -= 30;
            }

            //minimize distance to all other kings
            if(total_black_pieces + total_white_pieces < 8)
            {
                int total_distance = 0;
                int bring_loser_towards_center = 0;
                for (int i = 0; i < 8; i++)
                {
                    for (int jj = 0; jj < 4; jj++)
                    {
                        int j = 2*jj;
                        if(i % 2 == 0)
                            j = 2*jj + 1;

                        if (board[i][j] == 3)
                        {
                            for (int q = 0; q < 8; q++)
                            {
                                for (int rr = 0; rr < 4; rr++)
                                {
                                    int r = 2*rr;
                                    if(q % 2 == 0)
                                        r = 2*rr + 1;

                                    if (board[q][r] == 4)
                                    {
                                        total_distance += ((abs(i-q) + abs(j-r)));
                                    }
                                }
                            }
                        }
                        // If 1 king vs 2 this will help pull out the hiding king
                        if (board[i][j] == 4 && total_black_pieces + total_white_pieces < 4)
                        {
                            bring_loser_towards_center += (abs(3-i) + abs(3-j));
                        }
                    }
                }  
                score -= 10 * total_distance; 
                score -= 5 * bring_loser_towards_center;
            }
        }
        else if(whose_winning <= -200)
        {
            score += 10 * (total_black_pieces + total_white_pieces);

            if(total_black_pieces + total_white_pieces < 7)
            {
                if(board[1][0] == 3)
                    score += 30;
                if(board[0][1] == 3)
                    score += 30;
                if(board[7][6] == 3)
                    score += 30;
                if(board[6][7] == 3)
                    score += 30;
            }

            if(total_black_pieces + total_white_pieces < 8)
            {
                int total_distance = 0;
                int bring_loser_towards_center = 0;
                for (int i = 0; i < 8; i++)
                {
                    for (int jj = 0; jj < 4; jj++)
                    {
                        int j = 2*jj;
                        if(i % 2 == 0)
                            j = 2*jj + 1;

                        if (board[i][j] == 4)
                        {
                            for (int q = 0; q < 8; q++)
                            {
                                for (int rr = 0; rr < 4; rr++)
                                {
                                    int r = 2*rr;
                                    if(q % 2 == 0)
                                        r = 2*rr + 1;

                                    if (board[q][r] == 3)
                                    {
                                        total_distance += ((abs(i-q) + abs(j-r)));
                                    }
                                }
                            }
                        }
                        // If 1 king vs 2 this will help pull out the hiding king
                        if (board[i][j] == 3 && total_black_pieces + total_white_pieces < 4)
                        {
                            bring_loser_towards_center += abs(3-i) + abs(3-j);
                        }
                    }
                }  
                score += 10 * total_distance; 
                score += 5 * bring_loser_towards_center;
            }
        }

    }


    if (total_black_pieces == 0)
        score += (10000000 + (10000 * depth));
    else if (total_white_pieces == 0)
        score -= (10000000 + (10000 * depth));

    return score + rand() % 31 - 15;
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
    draw_checker++;
    for (int i = 0; i < move_list.size()-1; i++)
    {
        int piece = board_state[get<0>(move_list[i])][get<1>(move_list[i])];

        // Jump
        if (abs(abs(get<0>(move_list[i])) - abs(get<0>(move_list[i+1]))) == 2)
        {
            draw_checker = 0;
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

game::game(vector<vector<int>> board, bool move, double time)
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



