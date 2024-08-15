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


const std::vector<std::vector<std::vector<int>>> shape_alt_here = {
    {
        { 1, 0, 1, 1, 1, 2, 1, 3}, // I
        { 0, 1, 1, 0, 1, 1, 1, 2}, // T
        { 0, 2, 1, 0, 1, 1, 1, 2}, // L
        { 0, 0, 1, 2, 1, 0, 1, 1}, // J
        { 0, 0, 0, 1, 1, 0, 1, 1}, // O
        { 0, 1, 0, 2, 1, 0, 1, 1}, // S
        { 0, 0, 0, 1, 1, 1, 1, 2}  // Z
    },
    {
        { 0, 1, 1, 1, 2, 1, 3, 1}, // I
        { 0, 1, 1, 0, 1, 1, 2, 1}, // T
        { 0, 0, 0, 1, 1, 1, 2, 1}, // L
        { 0, 1, 1, 1, 2, 0, 2, 1}, // J
        { 0, 0, 1, 0, 0, 1, 1, 1}, // O
        { 0, 0, 1, 0, 1, 1, 2, 1}, // S
        { 0, 1, 1, 0, 1, 1, 2, 0}  // Z
    },
    { // shape[2]
        { 2, 0, 2, 1, 2, 2, 2, 3}, // I
        { 1, 0, 1, 1, 1, 2, 2, 1}, // T
        { 1, 0, 1, 1, 1, 2, 2, 0}, // L
        { 1, 0, 1, 1, 1, 2, 2, 2}, // J
        { 0, 0, 0, 1, 1, 0, 1, 1}, // O
        { 1, 1, 1, 2, 2, 0, 2, 1}, // S
        { 1, 0, 1, 1, 2, 1, 2, 2}  // Z
    },
    { // shape[3]
        { 2, 0, 2, 1, 2, 2, 2, 3}, // I
        { 1, 0, 1, 1, 1, 2, 2, 1}, // T
        { 1, 0, 1, 1, 1, 2, 2, 0}, // L
        { 1, 0, 1, 1, 1, 2, 2, 2}, // J
        { 0, 0, 0, 1, 1, 0, 1, 1}, // O
        { 1, 1, 1, 2, 2, 0, 2, 1}, // S
        { 1, 0, 1, 1, 2, 1, 2, 2}  // Z
    }
};


// const std::pair<int, int> kick_rules






bool actionFromKey(vector<vector<int>> &grid, Piece &piece, int key) {
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
        rotate(piece, grid);
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

void place_piece(Piece &piece, vector<vector<int>> &grid) {
    grid[piece.y + piece.shape_alt[0]][piece.x + piece.shape_alt[1]] = piece.shapeNum + 1;
    grid[piece.y + piece.shape_alt[2]][piece.x + piece.shape_alt[3]] = piece.shapeNum + 1;
    grid[piece.y + piece.shape_alt[4]][piece.x + piece.shape_alt[5]] = piece.shapeNum + 1;
    grid[piece.y + piece.shape_alt[6]][piece.x + piece.shape_alt[7]] = piece.shapeNum + 1;
}

bool checkCollision(Piece &piece, vector<vector<int>> &grid, int dx, int dy) {
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

bool checkCollisionRotate(vector<int> &shape_alt, vector<vector<int>> &grid, int x, int y) {
    return !(shape_alt[0] + y >= 0 && shape_alt[0] + y < 24 && shape_alt[1] + x >= 0 && shape_alt[1] + x < 10 && grid[shape_alt[0] + y][shape_alt[1] + x] == 0 &&
             shape_alt[2] + y >= 0 && shape_alt[2] + y < 24 && shape_alt[3] + x >= 0 && shape_alt[3] + x < 10 && grid[shape_alt[2] + y][shape_alt[3] + x] == 0 &&
             shape_alt[4] + y >= 0 && shape_alt[4] + y < 24 && shape_alt[5] + x >= 0 && shape_alt[5] + x < 10 && grid[shape_alt[4] + y][shape_alt[5] + x] == 0 &&
             shape_alt[6] + y >= 0 && shape_alt[6] + y < 24 && shape_alt[7] + x >= 0 && shape_alt[7] + x < 10 && grid[shape_alt[6] + y][shape_alt[7] + x] == 0);
}

bool move(Piece& piece, vector<vector<int>> &grid, int dx, int dy) {
    if (!checkCollision(piece, grid, dx, dy)) {
        piece.x += dx;
        piece.y += dy;
        return true;
    }
    return false;
}

bool rotate(Piece& piece, vector<vector<int>> &grid, bool counterClockwise) {

    int x = piece.x;
    int y = piece.y;
    int rotation = (piece.rotation + (counterClockwise ? 3 : 1)) & 0b11;

    vector<int> shape_alt = shape_alt_here[rotation][piece.shapeNum];

    if(!checkCollisionRotate(shape_alt, grid, x, y)){
        piece.shape_alt = shape_alt;
        piece.rotation = rotation;
        return true;
    }

    if(!checkCollisionRotate(shape_alt, grid, x+1, y)){
        piece.shape_alt = shape_alt;
        piece.rotation = rotation;
        piece.x += 1;
        return true;
    }

    if(!checkCollisionRotate(shape_alt, grid, x-1, y)){
        piece.shape_alt = shape_alt;
        piece.rotation = rotation;
        piece.x -= 1;
        return true;
    }

    if(!checkCollisionRotate(shape_alt, grid, x, y+1)){
        piece.shape_alt = shape_alt;
        piece.rotation = rotation;
        piece.y += 1;
        return true;
    }

    if(!checkCollisionRotate(shape_alt, grid, x+1, y+1)){
        piece.shape_alt = shape_alt;
        piece.rotation = rotation;
        piece.x += 1;
        piece.y += 1;
        return true;
    }

    if(!checkCollisionRotate(shape_alt, grid, x-1, y+1)){
        piece.shape_alt = shape_alt;
        piece.rotation = rotation;
        piece.x -= 1;
        piece.y += 1;
        return true;
    }

    if(!checkCollisionRotate(shape_alt, grid, x, y+2)){
        piece.shape_alt = shape_alt;
        piece.rotation = rotation;
        piece.y += 2;
        return true;
    }

    if(!checkCollisionRotate(shape_alt, grid, x+1, y+2)){
        piece.shape_alt = shape_alt;
        piece.rotation = rotation;
        piece.x += 1;
        piece.y += 2;
        return true;
    }

    if(!checkCollisionRotate(shape_alt, grid, x-1, y+2)){
        piece.shape_alt = shape_alt;
        piece.rotation = rotation;
        piece.x -= 1;
        piece.y += 2;
        return true;
    }

    // // initial spot
    // if (!checkCollision(piece, grid, 0, 0)) return true;

    // // left-right
    // if (!checkCollision(piece, grid, 1, 0)) {
    //     piece.x += 1;
    //     return true;
    // }
    // if (!checkCollision(piece, grid, -1, 0)) {
    //     piece.x -= 1;
    //     return true;
    // }

    // // 1 down
    // if (!checkCollision(piece, grid, 0, 1)) {
    //     piece.y += 1;
    //     return true;
    // }

    // // 1 down 1 side
    // if (!checkCollision(piece, grid, 1, 1)) {
    //     piece.x += 1;
    //     piece.y += 1;
    //     return true;
    // }
    // if (!checkCollision(piece, grid, -1, 1)) {
    //     piece.x -= 1;
    //     piece.y += 1;
    //     return true;
    // }

    // // 2 down
    // if (!checkCollision(piece, grid, 0, 2)) {
    //     piece.y += 2;
    //     return true;
    // }

    // // 2 down 1 side
    // if (!checkCollision(piece, grid, 1, 2)) {
    //     piece.x += 1;
    //     piece.y += 2;
    //     return true;
    // }
    // if (!checkCollision(piece, grid, -1, 2)) {
    //     piece.x -= 1;
    //     piece.y += 2;
    //     return true;
    // }

    return false;
}







void fastDrop(Piece& piece, vector<vector<int>> &grid) {
    while (!checkCollision(piece, grid, 0, 1)) {
        piece.y += 1;
    }
}

bool isTSpin(Piece& piece, vector<vector<int>> &grid, int lastMove) {
    if (piece.shapeNum != 1) return false;
    if (lastMove != ROTATE_CLOCKWISE && lastMove != ROTATE_COUNTER) return false;

    int corners = 0;
    corners += piece.x < 0 || grid[piece.y][piece.x] != 0;
    corners += piece.x + 2 >= 10 || grid[piece.y][piece.x + 2] != 0;
    corners += piece.x < 0 || piece.y + 2 >= 24 || grid[piece.y + 2][piece.x] != 0;
    corners += piece.x + 2 >= 10 || piece.y + 2 >= 24 || grid[piece.y + 2][piece.x + 2] != 0;

    if (corners >= 3) return true;

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
//     vector<vector<int>> grid(24, vector<int>(10, 0));
//     // random grid values

//     for (int i = 0; i < 24; i++) {
//         for (int j = 0; j < 10; j++) {
//             grid[i][j] = rand() & 1;
//         }
//     }

//     // init clock
//     clock_t start = clock();
//     // checkCollision x100000
//     for (int j = 0; j < 1000000; j++){
//         unordered_set<int> positions;
//         // int sampleGrid[28][14][4] = {0};
//         // bool sampleGrid[28][14][4] = {0};
//         // deque<int> positions;
//         // deque<int> x;
//         // deque<int> y;
//         // deque<int> rotation;
        
//         for (int i = 0; i < 100; i++) {
//             // sampleGrid[(rand() % 24) + 4][(rand() % 10) + 4] = (rand() % 4) + 1;
//             // sampleGrid[(rand() % 24) + 4][(rand() % 10) + 4][(rand() % 4)] = positionActionToBitString(rand() % 10, rand() % 24, rand() % 4, rand() % 7);
//             positions.insert(positionActionToBitString(rand() % 10, rand() % 24, rand() % 4, rand() % 7));
//             // positions.push_back(positionToBitString(rand() % 10, rand() % 24, rand() % 4));
//             // x.push_back(rand() % 10);
//             // y.push_back(rand() % 24);
//             // rotation.push_back(rand() % 4);
//         }

//         // for (int i = 0; i < 90; i++){
//         //     // int xx = x.front();
//         //     // x.pop_front();
//         //     // int yy = y.front();
//         //     // y.pop_front();
//         //     // int rotationr = rotation.front();
//         //     // rotation.pop_front();
//         //     // int pos = positions.front();
//         //     // positions.pop_front();
//         //     // int xx = (pos >> 6) & 63;
//         //     // int yy = pos & 63;
//         //     // int rotationr = pos >> 12;
//         // }
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