#ifndef COLLISION_H
#define COLLISION_H

#include <iostream>
#include <vector>
#include "Piece.h"

bool actionFromKey(std::vector<std::vector<int>>& grid, Piece& piece, int key);
void place_piece(Piece& piece, std::vector<std::vector<int>>& grid);
bool checkCollision(Piece& piece, std::vector<std::vector<int>>& grid, int x=0, int y=0);
bool move(Piece& piece, std::vector<std::vector<int>>& grid, int x, int y);
bool rotate(Piece& piece, std::vector<std::vector<int>>& grid, bool counterClockwise=false);
void fastDrop(Piece& piece, std::vector<std::vector<int>>& grid);
bool isTSpin(Piece& piece, std::vector<std::vector<int>>& grid, int lastMove);

#endif // COLLISION_H