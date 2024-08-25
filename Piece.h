#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <vector>



class Piece {
    public:
        int x, y, shapeNum, rotation;
        int** shape;
        int* shape_alt;
        int shape_size;
        int size;

        Piece(){
            x = 0;
            y = 0;
            shapeNum = -1;
            shape_size = 0;
            rotation = 0;
            shape = nullptr;
            shape_alt = nullptr;
            size = 0;
        }
        Piece(int shapeNum, int x, int y, int rotation);

        void rotate();
        void rotateBack();
        void resetPosition();
        void setRotation(int rot);
        void resetRotation();
        void setPosition(int x, int y);
        void setPositionRotation(int x, int y, int rot);

        // bool checkCollision(std::vector<std::vector<int>>& board, int x, int y);
};

class PieceRandomizer {
    public:
        std::vector<int> pieceCount;
        std::vector<int> pieceValue;
        int index;
        std::vector<Piece> pieces;
        int pieceIndex;

        PieceRandomizer();
        PieceRandomizer(int length);

        void reset_index();
        void setSequence(int length);
        Piece next();
        Piece nextPiece();
};

#endif // PIECE_H