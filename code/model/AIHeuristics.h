#ifndef AIHEURISTICS_H
#define AIHEURISTICS_H

#include "../tetris_lib/Piece.h"
#include <vector>

using namespace std;

vector<int> getBestAction(int board[24][10], Piece& piece, Piece* piece_hold, std::vector<Piece>& piece_queue, int piece_queue_index, int depth, vector<int>& genes, int combo, int b2b, int pending);
int evaluateBoard(vector<int>& genes, int board[24][10], Piece& piece, int lastAction, int* combo, int* b2b, int pending);

#endif // AIHEURISTICS_H