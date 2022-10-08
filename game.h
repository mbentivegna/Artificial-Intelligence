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

    // Declared from the start (black_move will alternate after that)
    bool black_move;
    int time_move;


    vector<vector<tuple<int, int>>> get_valid_moves_black();

    // Set up base position of board when creating game
    game();

    // Set up custom position of board when creating game
    game(vector<vector<int>> vect, bool move, int time);

    // Print current board state
    void print_board();



};

#endif //_GAME_H
