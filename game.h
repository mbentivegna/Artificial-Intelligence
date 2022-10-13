#ifndef _GAME_H
#define _GAME_H

#include <vector>
#include <string>
#include <tuple>

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
    int time_move;

    bool get_whose_move();

    void make_move(vector<tuple<int, int>> move_list);

    vector<vector<tuple<int, int>>> get_moves_given_whose_move(bool move);

    vector<vector<tuple<int, int>>> get_valid_moves_player1();

    vector<vector<tuple<int, int>>> get_valid_moves_player2();

    // Set up custom position of board when creating game
    game(vector<vector<int>> board, bool move, int time);

    // Print current board state
    void print_board();


    vector<vector<tuple<int, int>>> get_valid_jumps_player1(vector<vector<int>> board, int i, int j, int piece_val, vector<vector<tuple<int, int>>> current_jumps, vector<tuple<int, int>> tmpVect);

    vector<vector<tuple<int, int>>> get_valid_jumps_player2(vector<vector<int>> board, int i, int j, int piece_val, vector<vector<tuple<int, int>>> current_jumps, vector<tuple<int, int>> tmpVect);
};

#endif //_GAME_H
