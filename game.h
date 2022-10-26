#ifndef _GAME_H
#define _GAME_H

#include <vector>
#include <string>
#include <tuple>
#include <thread>
#include <chrono>

using namespace std;

class game {

  public:

    vector<vector<int>> board_state = {{ -1, 2, -1, 2, -1, 2, -1, 2},
                              {2, -1, 2, -1, 2, -1, 2, -1},
                              {-1, 2, -1, 2, -1, 2, -1, 2},
                              {0, -1, 0, -1, 0, -1, 0, -1},
                              {-1, 0, -1, 0, -1, 0, -1, 0},
                              {1, -1, 1, -1, 1, -1, 1, -1},
                              {-1, 1, -1, 1, -1, 1, -1, 1},
                              {1, -1, 1, -1, 1, -1, 1, -1}
                              };

    // Declared from the start (player1_move will alternate after that)
    bool player1_move;
    double time_move;
    int move_num = 0;


    //Alpha beta stuff
    int get_computer_move();

    vector<vector<int>> make_tmp_move(vector<tuple<int, int>> move_list, vector<vector<int>> board);

    int heuristic_function(vector<vector<int>> board, int total_depth, bool current_board_check);

    // returns the heuristic function output and its corresponding index
    tuple<int, int, bool> minimax(vector<vector<int>> board, int depth, bool player1, int alpha, int beta, clock_t start_time);

    void make_move(vector<tuple<int, int>> move_list);

    bool get_whose_move();


    vector<vector<tuple<int, int>>> get_moves_given_whose_move(bool move, vector<vector<int>> board);

    vector<vector<tuple<int, int>>> get_valid_moves_player1(vector<vector<int>> board);

    vector<vector<tuple<int, int>>> get_valid_moves_player2(vector<vector<int>> board);

    // Set up custom position of board when creating game
    game(vector<vector<int>> board, bool move, double time);

    // Print current board state
    vector<vector<int>> print_board();


    vector<vector<tuple<int, int>>> get_valid_jumps_player1(vector<vector<int>> board, int i, int j, int piece_val, vector<vector<tuple<int, int>>> current_jumps, vector<tuple<int, int>> tmpVect);

    vector<vector<tuple<int, int>>> get_valid_jumps_player2(vector<vector<int>> board, int i, int j, int piece_val, vector<vector<tuple<int, int>>> current_jumps, vector<tuple<int, int>> tmpVect);
};

#endif //_GAME_H
