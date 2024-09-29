#include <iostream>
#include <vector>
#include <cassert>

#include "Piece.h"
#include "constants.h"  

using namespace std;

int*** shape_alt_data = new int**[4]{
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


int**** shapes = new int***[4]{
    new int**[7]{
        new int*[4]{ // I
            new int[4]{0, 0, 0, 0},
            new int[4]{1, 1, 1, 1},
            new int[4]{0, 0, 0, 0},
            new int[4]{0, 0, 0, 0}
        },
        new int*[3]{ // T
            new int[3]{0, 1, 0},
            new int[3]{1, 1, 1},
            new int[3]{0, 0, 0}
        },
        new int*[3]{ // L
            new int[3]{0, 0, 1},
            new int[3]{1, 1, 1},
            new int[3]{0, 0, 0}
        },
        new int*[3]{ // J
            new int[3]{1, 0, 0},
            new int[3]{1, 1, 1},
            new int[3]{0, 0, 0}
        },
        new int*[2]{ // O
            new int[2]{1, 1},
            new int[2]{1, 1}
        },
        new int*[3]{ // S
            new int[3]{0, 1, 1},
            new int[3]{1, 1, 0},
            new int[3]{0, 0, 0}
        },
        new int*[3]{ // Z
            new int[3]{1, 1, 0},
            new int[3]{0, 1, 1},
            new int[3]{0, 0, 0}
        }
    },
    new int**[7]{
        new int*[4]{ // I
            new int[4]{0, 0, 1, 0},
            new int[4]{0, 0, 1, 0},
            new int[4]{0, 0, 1, 0},
            new int[4]{0, 0, 1, 0}
        },
        new int*[3]{ // T
            new int[3]{0, 1, 0},
            new int[3]{0, 1, 1},
            new int[3]{0, 1, 0}
        },
        new int*[3]{ // L
            new int[3]{0, 1, 0},
            new int[3]{0, 1, 0},
            new int[3]{0, 1, 1}
        },
        new int*[3]{ // J
            new int[3]{0, 1, 1},
            new int[3]{0, 1, 0},
            new int[3]{0, 1, 0}
        },
        new int*[2]{ // O
            new int[2]{1, 1},
            new int[2]{1, 1}
        },
        new int*[3]{ // S
            new int[3]{0, 1, 0},
            new int[3]{0, 1, 1},
            new int[3]{0, 0, 1}
        },
        new int*[3]{ // Z
            new int[3]{0, 0, 1},
            new int[3]{0, 1, 1},
            new int[3]{0, 1, 0}
        }
    },
    new int**[7]{ // shape[2]
        new int*[4]{ // I
            new int[4]{0, 0, 0, 0},
            new int[4]{0, 0, 0, 0},
            new int[4]{1, 1, 1, 1},
            new int[4]{0, 0, 0, 0}
        },
        new int*[3]{ // T
            new int[3]{0, 0, 0},
            new int[3]{1, 1, 1},
            new int[3]{0, 1, 0}
        },
        new int*[3]{ // L
            new int[3]{0, 0, 0},
            new int[3]{1, 1, 1},
            new int[3]{1, 0, 0}
        },
        new int*[3]{ // J
            new int[3]{0, 0, 0},
            new int[3]{1, 1, 1},
            new int[3]{0, 0, 1}
        },
        new int*[2]{ // O
            new int[2]{1, 1},
            new int[2]{1, 1}
        },
        new int*[3]{ // S
            new int[3]{0, 0, 0},
            new int[3]{0, 1, 1},
            new int[3]{1, 1, 0}
        },
        new int*[3]{ // Z
            new int[3]{0, 0, 0},
            new int[3]{1, 1, 0},
            new int[3]{0, 1, 1}
        }
    },
    new int**[7]{ // shape[3]
        new int*[4]{ // I
            new int[4]{0, 1, 0, 0},
            new int[4]{0, 1, 0, 0},
            new int[4]{0, 1, 0, 0},
            new int[4]{0, 1, 0, 0}
        },
        new int*[3]{ // T
            new int[3]{0, 1, 0},
            new int[3]{1, 1, 0},
            new int[3]{0, 1, 0}
        },
        new int*[3]{ // L
            new int[3]{1, 1, 0},
            new int[3]{0, 1, 0},
            new int[3]{0, 1, 0}
        },
        new int*[3]{ // J
            new int[3]{0, 1, 0},
            new int[3]{0, 1, 0},
            new int[3]{1, 1, 0}
        },
        new int*[2]{ // O
            new int[2]{1, 1},
            new int[2]{1, 1}
        },
        new int*[3]{ // S
            new int[3]{1, 0, 0},
            new int[3]{1, 1, 0},
            new int[3]{0, 1, 0}
        },
        new int*[3]{ // Z
            new int[3]{0, 1, 0},
            new int[3]{1, 1, 0},
            new int[3]{1, 0, 0}
        }
    }
};

int* shapesize = new int[7]{4, 3, 3, 3, 2, 3, 3};

Piece::Piece(int shapeNum, int x, int y, int rotation){
    // assert(shapeNum >= 0 && shapeNum < 7);
    this->shapeNum = shapeNum;
    this->x = x;
    this->y = y;
    this->rotation = rotation;
    this->size = shapesize[shapeNum];
    this->shape = shapes[rotation][shapeNum];
    this->shape_alt = shape_alt_data[rotation][shapeNum];
    this->shape_size = shapesize[shapeNum];
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
    this->x = 4;
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

void shuffle(int* arr, int n){
    for(int i = 0; i < n; i++){
        int j = rand() % n;
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}


void PieceRandomizer::setSequence(int length){
    // for(int i = 0; i < length; i++){
    //     this->pieces.push_back(this->next());
    // }
    int cnt = 0;
    int* arr = new int[7]{0, 1, 2, 3, 4, 5, 6};
    for(int i = 0; i < length; i++){
        if(cnt == 0){
            shuffle(arr, 7);
            cnt = 7;
        }
        cnt -= 1;
        // cout<<cnt<<endl;
        // for(int j = 0; j < 7; j++){
        //     cout<<arr[j]<<" ";
        // }
        // cout<<endl;
        // this->pieces.push_back(Piece(arr[cnt], 4, 0, 0));
        this->pieces.push_back(Piece(arr[cnt], 4, 0, 0));
    }
    delete[] arr;
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

    return Piece(num, 4, 0, 0);
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
//     int** shape = p.shape;
//     for(int i = 0; i < shapesize[p.shapeNum]; i++){
//         for(int j = 0; j < shapesize[p.shapeNum]; j++){
//             cout<<shape[i][j]<<" ";
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