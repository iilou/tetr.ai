#include <iostream>
#include <vector>

#include "Piece.h"
#include "constants.h"  

using namespace std;

const std::vector<std::vector<std::vector<int>>> shape_alt_data = {
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

/*

import numpy as np;
import random;
import copy;

# tetris shapes
shapes = []
shape_alt = []
for i in range(4):
    shape_alt.append([])
    for j in range(7):
        shape_alt[i].append([])


# rot 0
shapes.append([
    np.array([ # I
        [0, 0, 0, 0],
        [1, 1, 1, 1],
        [0, 0, 0, 0],
        [0, 0, 0, 0]
    ]),
    np.array([ # T
        [0, 1, 0],
        [1, 1, 1],
        [0, 0, 0]
    ]),
    np.array([ # L
        [0, 0, 1],
        [1, 1, 1],
        [0, 0, 0]
    ]),
    np.array([ # J
        [1, 0, 0],
        [1, 1, 1],
        [0, 0, 0]
    ]),
    np.array([ # O
        [1, 1],
        [1, 1]
    ]),
    np.array([ # S
        [0, 1, 1],
        [1, 1, 0],
        [0, 0, 0]
    ]),
    np.array([ # Z
        [1, 1, 0],
        [0, 1, 1],
        [0, 0, 0]
    ])
])
# rot90 (counter clockwise)
shapes.append([
    np.array([ # I
        [0, 1, 0, 0],
        [0, 1, 0, 0],
        [0, 1, 0, 0],
        [0, 1, 0, 0]
    ]),
    np.array([ # T
        [0, 1, 0],
        [1, 1, 0],
        [0, 1, 0]
    ]),
    np.array([ # L
        [1, 1, 0],
        [0, 1, 0],
        [0, 1, 0]
    ]),
    np.array([ # J
        [0, 1, 0],
        [0, 1, 0],
        [1, 1, 0]
    ]),
    np.array([ # O
        [1, 1],
        [1, 1]
    ]),
    np.array([ # S
        [1, 0, 0],
        [1, 1, 0],
        [0, 1, 0]
    ]),
    np.array([ # Z
        [0, 1, 0],
        [1, 1, 0],
        [1, 0, 0]
    ])
])
# rot180
shapes.append([
    np.array([ # I
        [0, 0, 0, 0],
        [0, 0, 0, 0],
        [1, 1, 1, 1],
        [0, 0, 0, 0]
    ]),
    np.array([ # T
        [0, 0, 0],
        [1, 1, 1],
        [0, 1, 0]
    ]),
    np.array([ # L
        [0, 0, 0],
        [1, 1, 1],
        [1, 0, 0]
    ]),
    np.array([ # J
        [0, 0, 0],
        [1, 1, 1],
        [0, 0, 1]
    ]),
    np.array([ # O
        [1, 1],
        [1, 1]
    ]),
    np.array([ # S
        [0, 0, 0],
        [0, 1, 1],
        [1, 1, 0]
    ]),
    np.array([ # Z
        [0, 0, 0],
        [1, 1, 0],
        [0, 1, 1]
    ])
])
# rot270
shapes.append([
    np.array([ # I
        [0, 0, 1, 0],
        [0, 0, 1, 0],
        [0, 0, 1, 0],
        [0, 0, 1, 0]
    ]),
    np.array([ # T
        [0, 1, 0],
        [0, 1, 1],
        [0, 1, 0]
    ]),
    np.array([ # L
        [0, 1, 0],
        [0, 1, 0],
        [0, 1, 1]
    ]),
    np.array([ # J
        [0, 1, 1],
        [0, 1, 0],
        [0, 1, 0]
    ]),
    np.array([ # O
        [1, 1],
        [1, 1]
    ]),
    np.array([ # S
        [0, 1, 0],
        [0, 1, 1],
        [0, 0, 1]
    ]),
    np.array([ # Z
        [0, 0, 1],
        [0, 1, 1],
        [0, 1, 0]
    ])
])

# shape alt -> 0 = x, 1 = y
for i in range(4):
    for j in range(7):
        for k in range(len(shapes[i][j])):
            for l in range(len(shapes[i][j][k])):
                if shapes[i][j][k][l] == 1:
                    shape_alt[i][j].append(k)
                    shape_alt[i][j].append(l)

shapes_2 = [
    np.array([ # I
        [1, 1, 1, 1]
    ]),
    np.array([ # T
        [0, 1, 0],
        [1, 1, 1]
    ]),
        np.array([ # L
        [0, 0, 1],
        [1, 1, 1]
    ]),
    np.array([ # J
        [1, 0, 0],
        [1, 1, 1]
    ]),
    np.array([ # O
        [1, 1],
        [1, 1]
    ]),
    np.array([ # S
        [0, 1, 1],
        [1, 1, 0]
    ]),
    np.array([ # Z
        [1, 1, 0],
        [0, 1, 1]
    ])
]

# tetris colors
colors = [
    # cyan
    (100, 255, 255),
    # purple
    (155, 55, 255),
    # orange
    (255, 155, 55),
    # blue
    (55, 155, 255),
    # yellow
    (255, 255, 55),
    # green
    (55, 255, 55),
    # red
    (255, 55, 55)
]

class PieceRandomizer:

    class Node:
        def __init__(self, value):
            self.value = value;
            self.next = None;

    def __init__(self):
        #array of 7
        self.pieceCount = [0, 0, 0, 0, 0, 0, 0];
        self.pieceValue = [-1, -1, -1, -1, -1, -1, -1, -1];
        self.index = 0;
        self.pieces = [];
    
    def __init__(self, length):
        self.pieceCount = [0, 0, 0, 0, 0, 0, 0];
        self.pieceValue = [-1, -1, -1, -1, -1, -1, -1, -1];
        self.index = 0;
        self.pieces = [];
        self.setSequence(length);
        self.pieceIndex = -1;
    
    def reset_index(self):
        self.pieceIndex = -1;
    
    def setSequence(self, length):
        for i in range(length):
            self.pieces.append(self.next());

    def next(self):
        num = random.randint(0, 6);
        for i in range(10): # fish for 0 count
            if(self.pieceCount[num] == 0):
                num = (num+1000)
                break;
            num = random.randint(0, 6);
        
        if(num < 100):
            for i in range(100):
                if(self.pieceCount[num] <= 2):
                    break;
                num = random.randint(0, 6);
        
        else:
            num = num - 1000;

        self.pieceCount[num] += 1;
        self.index = (self.index + 1) % 8;
        if(self.pieceValue[self.index] != -1):
            self.pieceCount[self.pieceValue[self.index]] -= 1;
        self.pieceValue[self.index] = num;

        # x -> middle
        return Piece(num, 5 - len(shapes[0][num][0]) // 2, 0);

    def nextPiece(self):
        self.pieceIndex = (self.pieceIndex + 1) % len(self.pieces);
        return self.pieces[self.pieceIndex];

    def newInstance(self):
        randomizer = PieceRandomizer(len(self.pieces));
        randomizer.pieces = copy.deepcopy(self.pieces);
        randomizer.pieceIndex = self.pieceIndex;
        return randomizer;


class Piece:
    def __init__(self, shapeNum, x, y, rotation = 0):
        self.shapeNum = shapeNum;
        self.shape = shapes[rotation][shapeNum];
        self.shape_alt = shape_alt[rotation][shapeNum];
        self.altShape = shapes_2[shapeNum];
        self.color = colors[shapeNum];
        self.x = x;
        self.y = y;
        self.exists = False;
        self.rotation = rotation;
        self.holdStatus = 0; # 0 = not held, 1 = held, 2 = used
        self.specialSpins = 0;

    def rotate(self):
        self.rotation = (self.rotation + 1) & 0b11;
        self.shape = shapes[self.rotation][self.shapeNum];
        self.shape_alt = shape_alt[self.rotation][self.shapeNum];

    def rotateBack(self):
        # self.rotation = (self.rotation + 3) % 4;
        self.rotation = (self.rotation + 3) & 0b11;
        self.shape = shapes[self.rotation][self.shapeNum];
        self.shape_alt = shape_alt[self.rotation][self.shapeNum];

    def resetPosition(self):
        self.x = 5 - len(shapes[self.shapeNum][0]) // 2;
        self.y = 0;
        self.rotation = 0;
        self.shape = shapes[0][self.shapeNum];
        self.shape_alt = shape_alt[0][self.shapeNum];

    def setRotation(self, rot):
        self.rotation = rot;
        self.shape = shapes[rot][self.shapeNum];
        self.shape_alt = shape_alt[rot][self.shapeNum];

    def resetRotation(self):
        self.rotation = 0;
        self.shape = shapes[0][self.shapeNum];
        self.shape_alt = shape_alt[0][self.shapeNum];

    def setPosition(self, x, y):
        self.x = x;
        self.y = y;

    def setPositionRotation(self, x, y, rot):
        self.x = x;
        self.y = y;
        self.rotation = rot;
        self.shape = shapes[rot][self.shapeNum];
        self.shape_alt = shape_alt[rot][self.shapeNum];

    

    def checkCollision(self, board):
        for y in range(len(self.shape)):
            for x in range(len(self.shape[y])):
                if(self.shape[y][x] == 1):
                    if(self.x + x < 0 or self.x + x >= len(board[0]) or self.y + y >= len(board)):
                        return True;
                    if(board[self.y + y][self.x + x] != -1):
                        return True;
        return False;
 */

// init shapes
std::vector<std::vector<std::vector<std::vector<int>>>> shapes = {
    {
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 0},
            {1, 1, 1},
            {0, 0, 0}
        },
        {
            {0, 0, 1},
            {1, 1, 1},
            {0, 0, 0}
        },
        {
            {1, 0, 0},
            {1, 1, 1},
            {0, 0, 0}
        },
        {
            {1, 1},
            {1, 1}
        },
        {
            {0, 1, 1},
            {1, 1, 0},
            {0, 0, 0}
        },
        {
            {1, 1, 0},
            {0, 1, 1},
            {0, 0, 0}
        }
    },
    {
        {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0}
        },
        {
            {0, 1, 0},
            {1, 1, 0},
            {0, 1, 0}
        },
        {
            {1, 1, 0},
            {0, 1, 0},
            {0, 1, 0}
        },
        {
            {0, 1, 0},
            {0, 1, 0},
            {1, 1, 0}
        },
        {
            {1, 1},
            {1, 1}
        },
        {
            {1, 0, 0},
            {1, 1, 0},
            {0, 1, 0}
        },
        {
            {0, 1, 0},
            {1, 1, 0},
            {1, 0, 0}
        }
    },
    {
        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0},
            {1, 1, 1},
            {0, 1, 0}
        },
        {
            {0, 0, 0},
            {1, 1, 1},
            {1, 0, 0}
        },
        {
            {0, 0, 0},
            {1, 1, 1},
            {0, 0, 1}
        },
        {
            {1, 1},
            {1, 1}
        },
        {
            {0, 0, 0},
            {0, 1, 1},
            {1, 1, 0}
        },
        {
            {0, 0, 0},
            {1, 1, 0},
            {0, 1, 1}
        }
    },
    {
        {
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0}
        },
        {
            {0, 1, 0},
            {0, 1, 1},
            {0, 1, 0}
        },
        {
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 1}
        },
        {
            {0, 1, 1},
            {0, 1, 0},
            {0, 1, 0}
        },
        {
            {1, 1},
            {1, 1}
        },
        {
            {0, 1, 0},
            {0, 1, 1},
            {0, 0, 1}
        },
        {
            {0, 0, 1},
            {0, 1, 1},
            {0, 1, 0}
        }
    }
};

Piece::Piece(int shapeNum, int x, int y, int rotation){
    this->shapeNum = shapeNum;
    this->x = x;
    this->y = y;
    this->rotation = rotation;
    this->shape = shapes[rotation][shapeNum];
    this->shape_alt = shape_alt_data[rotation][shapeNum];
}

void Piece::rotate(){
    this->rotation = (this->rotation + 1) & 0b11;
    this->shape = shapes[this->rotation][this->shapeNum];
    this->shape_alt = shape_alt_data[this->rotation][this->shapeNum];
}

void Piece::rotateBack(){
    this->rotation = (this->rotation + 3) & 0b11;
    this->shape = shapes[this->rotation][this->shapeNum];
    this->shape_alt = shape_alt_data[this->rotation][this->shapeNum];
}

void Piece::resetPosition(){
    this->x = 5 - shapes[0][this->shapeNum][0].size() / 2;
    this->y = 0;
    this->rotation = 0;
    this->shape = shapes[0][this->shapeNum];
    this->shape_alt = shape_alt_data[0][this->shapeNum];
}

void Piece::setRotation(int rot){
    this->rotation = rot;
    this->shape = shapes[rot][this->shapeNum];
    this->shape_alt = shape_alt_data[rot][this->shapeNum];
}

void Piece::resetRotation(){
    this->rotation = 0;
    this->shape = shapes[0][this->shapeNum];
    this->shape_alt = shape_alt_data[0][this->shapeNum];
}

void Piece::setPosition(int x, int y){
    this->x = x;
    this->y = y;
}

void Piece::setPositionRotation(int x, int y, int rot){
    this->x = x;
    this->y = y;
    this->rotation = rot;
    this->shape = shapes[rot][this->shapeNum];
    this->shape_alt = shape_alt_data[rot][this->shapeNum];
}

PieceRandomizer::PieceRandomizer(){
    this->pieceCount = {0, 0, 0, 0, 0, 0, 0};
    this->pieceValue = {-1, -1, -1, -1, -1, -1, -1, -1};
    this->index = 0;
    this->pieces = {};
    this->pieceIndex = -1;
    this->setSequence(70);
}

PieceRandomizer::PieceRandomizer(int length){
    this->pieceCount = {0, 0, 0, 0, 0, 0, 0};
    this->pieceValue = {-1, -1, -1, -1, -1, -1, -1, -1};
    this->index = 0;
    this->pieces = {};
    this->setSequence(length);
    this->pieceIndex = -1;
}

void PieceRandomizer::reset_index(){
    this->pieceIndex = -1;
}

void PieceRandomizer::setSequence(int length){
    for(int i = 0; i < length; i++){
        this->pieces.push_back(this->next());
    }
}

Piece PieceRandomizer::next(){
    int num = rand() % 7;
    for(int i = 0; i < 10; i++){
        if(this->pieceCount[num] == 0){
            num = (num + 1000);
            break;
        }
        num = rand() % 7;
    }
    if(num < 100){
        for(int i = 0; i < 100; i++){
            if(this->pieceCount[num] <= 2){
                break;
            }
            num = rand() % 7;
        }
    }else{
        num = num - 1000;
    }

    this->pieceCount[num] += 1;
    this->index = (this->index + 1) % 8;
    if(this->pieceValue[this->index] != -1){
        this->pieceCount[this->pieceValue[this->index]] -= 1;
    }
    this->pieceValue[this->index] = num;

    return Piece(num, 5 - shapes[0][num][0].size() / 2, 0, 0);
}

Piece PieceRandomizer::nextPiece(){
    this->pieceIndex = (this->pieceIndex + 1) % this->pieces.size();
    return this->pieces[this->pieceIndex];
}


    // void test(){
    //     PieceRandomizer pr = PieceRandomizer(70);

    //     Piece p = pr.nextPiece();
    //     cout<<p.x<<endl;
    //     cout<<p.y<<endl;
    //     cout<<p.shapeNum<<endl;
    //     cout<<p.rotation<<endl;
    //     for (int i = 0; i < p.shape.size(); i++){
    //         for (int j = 0; j < p.shape[i].size(); j++){
    //             cout<<p.shape[i][j]<<" ";
    //         }
    //         cout<<endl;
    //     }
    //     for (int i = 0; i < 8; i++){
    //         cout<<p.shape_alt[i]<<" ";
    //     }
    //     cout<<endl;

    //     cout<<rand()<<endl;
    //     return;
    // }



    // int main(){
    //     test();
    //     return 0;
    // }