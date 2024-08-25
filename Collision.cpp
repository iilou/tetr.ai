/*
from typing import final;
from Piece import *;

CLOCKWISE: final = 1;
COUNTERCLOCKWISE: final = -1;


MOVE_LEFT = 0;
MOVE_RIGHT = 1;
SOFT_DROP = 2;
ROTATE_CLOCKWISE = 3;
ROTATE_COUNTER = 4;
HARD_DROP = 5;
SOFT_DROP_FORCE = 6;

def print_array(arr): 
    cnt = 0;
    for i in arr: 
        print("#",i);
        cnt+=1;







def actionFromKey(grid, piece, key):
    """
    return True if action results in locking piece
    """
    # print("ACTION");
    # print(key);
    # print_array(grid);
    # print(piece.shapeNum, piece.x, piece.y, piece.shape_alt, piece.rotation);
    # print_array(piece.shape);
    if key == MOVE_LEFT:
        move(piece, grid, -1, 0);
        return False;
    if key == MOVE_RIGHT:
        move(piece, grid, 1, 0);
        return False;
    if key == SOFT_DROP:
        move(piece, grid, 0, 1);
        return False;
    if key == ROTATE_CLOCKWISE:
        rotate(piece, grid);
        return False;
    if key == ROTATE_COUNTER:
        rotate(piece, grid, True);
        return False;
    if key == HARD_DROP:
        fastDrop(piece, grid);
        return True;    
    if key == SOFT_DROP_FORCE:
        return not move(piece, grid, 0, 1);
    return False;


def softDrop(piece, grid, force=False):
    if force:
        return fastDrop(piece, grid);
    return move(piece, grid, 0, 1);







def checkCollisionRaw(grid, x, y, shape_alt, dx=0, dy=0):
    return not (shape_alt[0] + y + dy >= 0 and shape_alt[0] + y + dy < 24 and
                shape_alt[1] + x + dx >= 0 and shape_alt[1] + x + dx < 10 and
                grid[shape_alt[0] + y + dy][shape_alt[1] + x + dx] == 0 and
                shape_alt[2] + y + dy >= 0 and shape_alt[2] + y + dy < 24 and
                shape_alt[3] + x + dx >= 0 and shape_alt[3] + x + dx < 10 and
                grid[shape_alt[2] + y + dy][shape_alt[3] + x + dx] == 0 and
                shape_alt[4] + y + dy >= 0 and shape_alt[4] + y + dy < 24 and
                shape_alt[5] + x + dx >= 0 and shape_alt[5] + x + dx < 10 and
                grid[shape_alt[4] + y + dy][shape_alt[5] + x + dx] == 0 and
                shape_alt[6] + y + dy >= 0 and shape_alt[6] + y + dy < 24 and
                shape_alt[7] + x + dx >= 0 and shape_alt[7] + x + dx < 10 and
                grid[shape_alt[6] + y + dy][shape_alt[7] + x + dx] == 0);

def checkCollision(piece, grid, dx=0, dy=0):
    """
    if the piece is colliding with the grid : return True
    else : return False
    """
    
    return not (piece.shape_alt[0] + piece.y + dy >= 0 and piece.shape_alt[0] + piece.y + dy < 24 and 
            piece.shape_alt[1] + piece.x + dx >= 0 and piece.shape_alt[1] + piece.x + dx < 10 and 
            grid[piece.shape_alt[0] + piece.y + dy][piece.shape_alt[1] + piece.x + dx] == 0 and 
            piece.shape_alt[2] + piece.y + dy >= 0 and piece.shape_alt[2] + piece.y + dy < 24 and
            piece.shape_alt[3] + piece.x + dx >= 0 and piece.shape_alt[3] + piece.x + dx < 10 and
            grid[piece.shape_alt[2] + piece.y + dy][piece.shape_alt[3] + piece.x + dx] == 0 and 
            piece.shape_alt[4] + piece.y + dy >= 0 and piece.shape_alt[4] + piece.y + dy < 24 and
            piece.shape_alt[5] + piece.x + dx >= 0 and piece.shape_alt[5] + piece.x + dx < 10 and
            grid[piece.shape_alt[4] + piece.y + dy][piece.shape_alt[5] + piece.x + dx] == 0 and 
            piece.shape_alt[6] + piece.y + dy >= 0 and piece.shape_alt[6] + piece.y + dy < 24 and
            piece.shape_alt[7] + piece.x + dx >= 0 and piece.shape_alt[7] + piece.x + dx < 10 and
            grid[piece.shape_alt[6] + piece.y + dy][piece.shape_alt[7] + piece.x + dx] == 0);

def move(piece, grid, dx, dy):
    if not checkCollision(piece, grid, dx, dy):
        piece.x += dx;
        piece.y += dy;
        return True;
    return False;

def rotate(piece, grid, counterClockwise=False):
    # direction: 1 = clockwise, -1 = counterclockwise

    if not counterClockwise:
        piece.rotate();
    else:
        piece.rotateBack();

    if not checkCollision(piece, grid, 0, 0):
        return True;

    # left-right
    if not checkCollision(piece, grid, 1, 0):
        piece.x += 1;
        return True;
    if not checkCollision(piece, grid, -1, 0):
        piece.x -= 1;
        return True;

    # 1 down
    if not checkCollision(piece, grid, 0, 1):
        piece.y += 1;
        return True;

    if not checkCollision(piece, grid, 1, 1):
        piece.x += 1;
        piece.y += 1;
        return True;
    if not checkCollision(piece, grid, -1, 1):
        piece.x -= 1;
        piece.y += 1;
        return True;

    if not checkCollision(piece, grid, 0, 2):
        piece.y += 2;
        return True;

    if not checkCollision(piece, grid, 1, 2):
        piece.x += 1;
        piece.y += 2;
        return True;
    if not checkCollision(piece, grid, -1, 2):
        piece.x -= 1;
        piece.y += 2;
        return True;

    if counterClockwise:
        piece.rotate();
    else:
        piece.rotateBack();

    return False;   

def fastDrop(piece, grid):
    while not checkCollision(piece, grid, 0, 1):
        piece.y += 1;
    return piece;

def isTSpin(piece, grid, lastMove):
    if piece.shapeNum != 1:
        # print("T-Spin: shapeNum is not 1");
        return False;
    if lastMove != 3 and lastMove != 4:
        # print("T-Spin: lastMove is not 3 or 4", lastMove);
        return False;
    # 3 corner check
    corners = 0;
    corners += piece.x < 0 or grid[piece.y][piece.x] != 0;
    corners += piece.x + 2 >= 10 or grid[piece.y][piece.x + 2] != 0;
    corners += piece.x < 0 or piece.y + 2 >= 24 or grid[piece.y + 2][piece.x] != 0;
    corners += piece.x + 2 >= 10 or piece.y + 2 >= 24 or grid[piece.y + 2][piece.x + 2] != 0;


    if corners >= 3:
        return True;
    
    return False;


def doNothing(piece, grid):
    return False;

def check_game_over(placedPiece):
    return placedPiece.y+placedPiece.shape_alt[1] < 4 or placedPiece.y+placedPiece.shape_alt[3] < 4 or placedPiece.y+placedPiece.shape_alt[5] < 4 or placedPiece.y+placedPiece.shape_alt[7] < 4;


def place_piece(piece:'Piece', grid):
    grid[piece.y + piece.shape_alt[0]][piece.x + piece.shape_alt[1]] = piece.shapeNum+1;
    grid[piece.y + piece.shape_alt[2]][piece.x + piece.shape_alt[3]] = piece.shapeNum+1;
    grid[piece.y + piece.shape_alt[4]][piece.x + piece.shape_alt[5]] = piece.shapeNum+1;
    grid[piece.y + piece.shape_alt[6]][piece.x + piece.shape_alt[7]] = piece.shapeNum+1;

def remove_piece(piece:'Piece', grid):
    grid[piece.y + piece.shape_alt[0]][piece.x + piece.shape_alt[1]] = 0;
    grid[piece.y + piece.shape_alt[2]][piece.x + piece.shape_alt[3]] = 0;
    grid[piece.y + piece.shape_alt[4]][piece.x + piece.shape_alt[5]] = 0;
    grid[piece.y + piece.shape_alt[6]][piece.x + piece.shape_alt[7]] = 0;
 */


#include <iostream>
#include <vector>

#include <cstdio>
#include <ctime>

#include "Collision.h"
#include "Piece.h"
#include "constants.h"

#include <unordered_set>
#include <deque>

using namespace std;


// const std::vector<std::vector<std::vector<int>>> shape_alt_here = {
//     {
//         { 1, 0, 1, 1, 1, 2, 1, 3}, // I
//         { 0, 1, 1, 0, 1, 1, 1, 2}, // T
//         { 0, 2, 1, 0, 1, 1, 1, 2}, // L
//         { 0, 0, 1, 2, 1, 0, 1, 1}, // J
//         { 0, 0, 0, 1, 1, 0, 1, 1}, // O
//         { 0, 1, 0, 2, 1, 0, 1, 1}, // S
//         { 0, 0, 0, 1, 1, 1, 1, 2}  // Z
//     },
//     {
//         { 0, 1, 1, 1, 2, 1, 3, 1}, // I
//         { 0, 1, 1, 0, 1, 1, 2, 1}, // T
//         { 0, 0, 0, 1, 1, 1, 2, 1}, // L
//         { 0, 1, 1, 1, 2, 0, 2, 1}, // J
//         { 0, 0, 1, 0, 0, 1, 1, 1}, // O
//         { 0, 0, 1, 0, 1, 1, 2, 1}, // S
//         { 0, 1, 1, 0, 1, 1, 2, 0}  // Z
//     },
//     { // shape[2]
//         { 2, 0, 2, 1, 2, 2, 2, 3}, // I
//         { 1, 0, 1, 1, 1, 2, 2, 1}, // T
//         { 1, 0, 1, 1, 1, 2, 2, 0}, // L
//         { 1, 0, 1, 1, 1, 2, 2, 2}, // J
//         { 0, 0, 0, 1, 1, 0, 1, 1}, // O
//         { 1, 1, 1, 2, 2, 0, 2, 1}, // S
//         { 1, 0, 1, 1, 2, 1, 2, 2}  // Z
//     },
//     { // shape[3]
//         { 2, 0, 2, 1, 2, 2, 2, 3}, // I
//         { 1, 0, 1, 1, 1, 2, 2, 1}, // T
//         { 1, 0, 1, 1, 1, 2, 2, 0}, // L
//         { 1, 0, 1, 1, 1, 2, 2, 2}, // J
//         { 0, 0, 0, 1, 1, 0, 1, 1}, // O
//         { 1, 1, 1, 2, 2, 0, 2, 1}, // S
//         { 1, 0, 1, 1, 2, 1, 2, 2}  // Z
//     }
// };


// const std::pair<int, int> kick_rules

int*** shape_alt_data_here = new int**[4]{
new int*[7]{
new int[8]{1, 0, 1, 1, 1, 2, 1, 3, },
new int[8]{0, 1, 1, 0, 1, 1, 1, 2, },
new int[8]{0, 2, 1, 0, 1, 1, 1, 2, },
new int[8]{0, 0, 1, 0, 1, 1, 1, 2, },
new int[8]{0, 0, 0, 1, 1, 0, 1, 1, },
new int[8]{0, 1, 0, 2, 1, 0, 1, 1, },
new int[8]{0, 0, 0, 1, 1, 1, 1, 2, },
},
new int*[7]{
new int[8]{0, 2, 1, 2, 2, 2, 3, 2, },
new int[8]{0, 1, 1, 1, 1, 2, 2, 1, },
new int[8]{0, 1, 1, 1, 2, 1, 2, 2, },
new int[8]{0, 1, 0, 2, 1, 1, 2, 1, },
new int[8]{0, 0, 0, 1, 1, 0, 1, 1, },
new int[8]{0, 1, 1, 1, 1, 2, 2, 2, },
new int[8]{0, 2, 1, 1, 1, 2, 2, 1, },
},
new int*[7]{
new int[8]{2, 0, 2, 1, 2, 2, 2, 3, },
new int[8]{1, 0, 1, 1, 1, 2, 2, 1, },
new int[8]{1, 0, 1, 1, 1, 2, 2, 0, },
new int[8]{1, 0, 1, 1, 1, 2, 2, 2, },
new int[8]{0, 0, 0, 1, 1, 0, 1, 1, },
new int[8]{1, 1, 1, 2, 2, 0, 2, 1, },
new int[8]{1, 0, 1, 1, 2, 1, 2, 2, },
},
new int*[7]{
new int[8]{0, 1, 1, 1, 2, 1, 3, 1, },
new int[8]{0, 1, 1, 0, 1, 1, 2, 1, },
new int[8]{0, 0, 0, 1, 1, 1, 2, 1, },
new int[8]{0, 1, 1, 1, 2, 0, 2, 1, },
new int[8]{0, 0, 0, 1, 1, 0, 1, 1, },
new int[8]{0, 0, 1, 0, 1, 1, 2, 1, },
new int[8]{0, 1, 1, 0, 1, 1, 2, 0, },
},
};




bool actionFromKey(int grid[24][10], Piece &piece, int key) {
    if (key == MOVE_LEFT) {
        move(piece, grid, -1, 0);
        return false;
    }
    if (key == MOVE_RIGHT) {
        move(piece, grid, 1, 0);
        return false;
    }
    if (key == SOFT_DROP) {
        move(piece, grid, 0, 1);
        return false;
    }
    if (key == ROTATE_CLOCKWISE) {
        rotate(piece, grid, false);
        return false;
    }
    if (key == ROTATE_COUNTER) {
        rotate(piece, grid, true);
        return false;
    }
    if (key == HARD_DROP) {
        fastDrop(piece, grid);
        return true;
    }
    if (key == SOFT_DROP_FORCE) {
        return !move(piece, grid, 0, 1);
    }
    return false;
}

void place_piece(Piece &piece, int grid[24][10]) {
    grid[piece.y + piece.shape_alt[0]][piece.x + piece.shape_alt[1]] = piece.shapeNum + 1;
    grid[piece.y + piece.shape_alt[2]][piece.x + piece.shape_alt[3]] = piece.shapeNum + 1;
    grid[piece.y + piece.shape_alt[4]][piece.x + piece.shape_alt[5]] = piece.shapeNum + 1;
    grid[piece.y + piece.shape_alt[6]][piece.x + piece.shape_alt[7]] = piece.shapeNum + 1;
}

bool checkCollision(Piece &piece, int grid[24][10], int dx, int dy) {
    return !(piece.shape_alt[0] + piece.y + dy >= 0 && piece.shape_alt[0] + piece.y + dy < 24 &&
             piece.shape_alt[1] + piece.x + dx >= 0 && piece.shape_alt[1] + piece.x + dx < 10 &&
             grid[piece.shape_alt[0] + piece.y + dy][piece.shape_alt[1] + piece.x + dx] == 0 &&
             piece.shape_alt[2] + piece.y + dy >= 0 && piece.shape_alt[2] + piece.y + dy < 24 &&
             piece.shape_alt[3] + piece.x + dx >= 0 && piece.shape_alt[3] + piece.x + dx < 10 &&
             grid[piece.shape_alt[2] + piece.y + dy][piece.shape_alt[3] + piece.x + dx] == 0 &&
             piece.shape_alt[4] + piece.y + dy >= 0 && piece.shape_alt[4] + piece.y + dy < 24 &&
             piece.shape_alt[5] + piece.x + dx >= 0 && piece.shape_alt[5] + piece.x + dx < 10 &&
             grid[piece.shape_alt[4] + piece.y + dy][piece.shape_alt[5] + piece.x + dx] == 0 &&
             piece.shape_alt[6] + piece.y + dy >= 0 && piece.shape_alt[6] + piece.y + dy < 24 &&
             piece.shape_alt[7] + piece.x + dx >= 0 && piece.shape_alt[7] + piece.x + dx < 10 &&
             grid[piece.shape_alt[6] + piece.y + dy][piece.shape_alt[7] + piece.x + dx] == 0);
}

bool checkCollisionRotate(int shape_alt[8], int grid[24][10], int x, int y) {
    return !(shape_alt[0] + y >= 0 && shape_alt[0] + y < 24 && shape_alt[1] + x >= 0 && shape_alt[1] + x < 10 && grid[shape_alt[0] + y][shape_alt[1] + x] == 0 &&
             shape_alt[2] + y >= 0 && shape_alt[2] + y < 24 && shape_alt[3] + x >= 0 && shape_alt[3] + x < 10 && grid[shape_alt[2] + y][shape_alt[3] + x] == 0 &&
             shape_alt[4] + y >= 0 && shape_alt[4] + y < 24 && shape_alt[5] + x >= 0 && shape_alt[5] + x < 10 && grid[shape_alt[4] + y][shape_alt[5] + x] == 0 &&
             shape_alt[6] + y >= 0 && shape_alt[6] + y < 24 && shape_alt[7] + x >= 0 && shape_alt[7] + x < 10 && grid[shape_alt[6] + y][shape_alt[7] + x] == 0);
}

bool move(Piece& piece, int grid[24][10], int dx, int dy) {
    if (!checkCollision(piece, grid, dx, dy)) {
        piece.x += dx;
        piece.y += dy;
        return true;
    }
    return false;
}


// kick_rules rot(4) -> rot(4) | shape(7) | set(5) | 
int* kick_rules_0 = new int[64]{ // I
    -2, 0, 1, 0, -2, -1, 1, 2, // 0 -> 1
    -1, 0, 2, 0, -1, 2, 2, -1, // 0 -> 3
    -1, 0, 2, 0, -1, 2, 2, -1, // 1 -> 2
    2, 0, -1, 0, 2, 1, -1, -2, // 1 -> 0
    2, 0, -1, 0, 2, 1, -1, -2, // 2 -> 3
    1, 0, -2, 0, 1, -2, -2, 1, // 2 -> 1
    1, 0, -2, 0, 1, -2, -2, 1, // 3 -> 0
    -2, 0, 1, 0, -2, -1, 1, 2, // 3 -> 2
};
int* kick_rules_1 = new int[64]{// J, L, S, T, Z
    -1, 0, -1, 1, 0, -2, -1, -2, // 0 -> 1
    1, 0, 1, 1, 0, -2, 1, -2, // 0 -> 3
    1, 0, 1, 1, 0, -2, 1, -2, // 1 -> 2
    1, 0, 1, -1, 0, 2, 1, 2, // 1 -> 0 
    1, 0, 1, 1, 0, -2, 1, -2, // 2 -> 3
    -1, 0, -1, -1, 0, 2, -1, 2, // 2 -> 1
    -1, 0, -1, -1, 0, 2, -1, 2, // 3 -> 0
    -1, 0, -1, -1, 0, 2, -1, 2, // 3 -> 2
};

bool rotate(Piece& piece, int grid[24][10], bool counterClockwise) {

    if(!counterClockwise) piece.rotate();
    else piece.rotateBack();

    if (!checkCollision(piece, grid, 0, 0)) {
        return true;
    }

    int shape = piece.shapeNum;

    if(shape == 4) return false;

    int rotation = piece.rotation*2 + counterClockwise;
    int* kick;
    if(shape == 0) kick = kick_rules_0;
    else kick = kick_rules_1;

    if(!checkCollision(piece, grid, kick[rotation*8], kick[rotation*8+1])){
        piece.x += kick[rotation*8];
        piece.y += kick[rotation*8+1];
        return true;
    }
    if(!checkCollision(piece, grid, kick[rotation*8+2], kick[rotation*8+3])){
        piece.x += kick[rotation*8+2];
        piece.y += kick[rotation*8+3];
        return true;
    }
    if(!checkCollision(piece, grid, kick[rotation*8+4], kick[rotation*8+5])){
        piece.x += kick[rotation*8+4];
        piece.y += kick[rotation*8+5];
        return true;
    }
    if(!checkCollision(piece, grid, kick[rotation*8+6], kick[rotation*8+7])){
        piece.x += kick[rotation*8+6];
        piece.y += kick[rotation*8+7];
        return true;
    }

    if(counterClockwise) piece.rotate();
    else piece.rotateBack();

    return false;
}







void fastDrop(Piece& piece, int grid[24][10]) {
    while (!checkCollision(piece, grid, 0, 1)) {
        piece.y += 1;
    }
}

bool isTSpin(Piece& piece, int grid[24][10], int lastMove) {
    if (piece.shapeNum != 1) return false;
    if (lastMove != ROTATE_CLOCKWISE && lastMove != ROTATE_COUNTER) return false;

    int corners = 0;
    corners += piece.x < 0 || grid[piece.y][piece.x] != 0;
    corners += piece.x + 2 >= 10 || grid[piece.y][piece.x + 2] != 0;
    corners += piece.x < 0 || piece.y + 2 >= 24 || grid[piece.y + 2][piece.x] != 0;
    corners += piece.x + 2 >= 10 || piece.y + 2 >= 24 || grid[piece.y + 2][piece.x + 2] != 0;

    if (corners >= 3) {
        // std::cout<<"T-Spin"<<std::endl;
        return true;
    }

    return false;
}

// int positionToBitString(int x, int y, int rotation){
//     return (rotation << 12) | ((x+4) << 6) | (y+4);
// }


// int positionActionToBitString(int x, int y, int rotation, int action){
//     return (action << 16) | (rotation << 12) | ((x+4) << 6) | (y+4);
// }


// void test(){
//     PieceRandomizer pr(100);
//     Piece piece = pr.nextPiece();

//     //create empty 24x10 grid
//     // vector<vector<int>> grid(24, vector<int>(10, 0));
//     int grid[24][10] = {0};
//     // random grid values

//     for (int i = 0; i < 24; i++) {
//         for (int j = 0; j < 10; j++) {
//             grid[i][j] = rand() & 1;
//         }
//     }

//     // init clock
//     clock_t start = clock();
//     for(int i = 0; i < 1000000; i++){
//         // checkCollision(piece, grid, 2, 14);
//         rotate(piece, grid, false);
//     }
//     // end clock
//     clock_t end = clock();
//     // print time in ms
//     cout << "Time: " << (end - start) * 1000 / CLOCKS_PER_SEC << "ms" << endl;
// }


// int main() {
//     test();
//     cout << "Hello World!" << endl;
//     return 0;
// }