#ifndef AIHEURISTICS_H
#define AIHEURISTICS_H

#include "Piece.h"
#include <vector>

int evaluateBoard(std::vector<int>& genes, std::vector<std::vector<int>>& board, Piece& piece, int lastAction);
std::vector<int> getBestAction(std::vector<std::vector<int>>& board, Piece& piece, std::vector<int>& genes, int combo, int b2b);

#endif // AIHEURISTICS_H