/*
Michael Bentivegna
Artificial Intelligence 
Checkers
*/

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

    // Default board state
    vector<vector<int>> board_state =  {{-1, 2, -1, 2, -1, 2, -1, 2},
                                        {2, -1, 2, -1, 2, -1, 2, -1},
                                        {-1, 2, -1, 2, -1, 2, -1, 2},
                                        {0, -1, 0, -1, 0, -1, 0, -1},
                                        {-1, 0, -1, 0, -1, 0, -1, 0},
                                        {1, -1, 1, -1, 1, -1, 1, -1},
                                        {-1, 1, -1, 1, -1, 1, -1, 1},
                                        {1, -1, 1, -1, 1, -1, 1, -1}
                                        };

    // True if player 1's turn
    bool player1_move;

    // Time for computer to move
    double time_move;

    // Initialize custom board, move timing, and whose move it is to start
    game(vector<vector<int>> board, bool move, double time);

    // Print current board state and whose turn it is
    vector<vector<int>> print_board();

    // Mission control for the computer
    // Returns the index of the best move in the move list
    int get_computer_move();

    // Determines if the position is a special 2v1 with kings on the board
    // True if in the special case
    bool two_on_one_endgame();

    // Function for making a move in a temporary board
    // Returns the temporary board after the move has been made
    vector<vector<int>> make_tmp_move(vector<tuple<int, int>> move_list, vector<vector<int>> board);

    // Heuristic for evaluating a current position
    // Positive values indicate that player 1 is winning
    int heuristic_function(vector<vector<int>> board, int total_depth, bool player1_turn);

    // Min and max function combined into 1
    // Recursively calls itself to a provided depth limit
    // Returns the heuristic value, index, and if the time limit ran out mid search
    tuple<int, int, bool> minimax(vector<vector<int>> board, int depth, bool player1, int alpha, int beta, clock_t start_time);

    // Make move on real board
    // Swaps whose move it is and adds one to draw checker
    void make_move(vector<tuple<int, int>> move_list);

    // Function to call when trying to find all moves given a current position
    // Calls either the get_valid_moves_player1 or get_valid_moves_player2 function based on whose move is passed in to the function
    vector<vector<tuple<int, int>>> get_moves_given_whose_move(bool move, vector<vector<int>> board);

    // Function to determine what valid moves are for each player
    // If jumps are found there, it no longer checks for regular moves -> jumps are returned
    // If no jumps are found -> regular moves are returned
    vector<vector<tuple<int, int>>> get_valid_moves_player1(vector<vector<int>> board);
    vector<vector<tuple<int, int>>> get_valid_moves_player2(vector<vector<int>> board);

    // Finds valid jumps and is recursively calls itself to check if double jumps are possible
    // Each recursive call branches has the potential to add a new move to valid move vector
    // However, only moves that end in a position not eligible for another double jump are returned (see the checker variable)
    vector<vector<tuple<int, int>>> get_valid_jumps_player1(vector<vector<int>> board, int i, int j, int piece_val, vector<vector<tuple<int, int>>> current_jumps, vector<tuple<int, int>> tmpVect);
    vector<vector<tuple<int, int>>> get_valid_jumps_player2(vector<vector<int>> board, int i, int j, int piece_val, vector<vector<tuple<int, int>>> current_jumps, vector<tuple<int, int>> tmpVect);

};

#endif //_GAME_H
