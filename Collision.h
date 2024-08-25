#ifndef COLLISION_H
#define COLLISION_H

#include <iostream>
#include <vector>
#include "Piece.h"

// int grid[24][10] 

bool actionFromKey(int grid[24][10], Piece& piece, int key);
void place_piece(Piece& piece, int grid[24][10]);
bool checkCollision(Piece& piece, int grid[24][10], int x=0, int y=0);
bool move(Piece& piece, int grid[24][10], int x, int y);
bool rotate(Piece& piece, int grid[24][10], bool counterClockwise=false);
void fastDrop(Piece& piece, int grid[24][10]);
bool isTSpin(Piece& piece, int grid[24][10], int lastMove);

#endif // COLLISION_H