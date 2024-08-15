
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

#include <cstdio>
#include <ctime>

#include "AIModel.h"
#include "Piece.h"
#include "Collision.h"
#include "constants.h"
#include "AIHeuristics.h"

using namespace std;

int positionToBitString(int x, int y, int rotation){
    return (rotation << 12) | ((x+4) << 6) | (y+4);
}

int positionActionToBitString(int x, int y, int rotation, int action){
    return (action << 16) | (rotation << 12) | ((x+4) << 6) | (y+4);
}

void swapPiece(Piece* a, Piece* b){
    Piece temp = *a;
    *a = *b;
    *b = temp;
}

double times[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int evaluateBoard(vector<int>& genes, vector<vector<int>>& board, Piece& piece, int lastAction, int combo, int b2b){
    int clear = 0;
    int tclear = 0;
    bool isT = isTSpin(piece, board, lastAction);

    bool to_clear[24] = {false};

    // put piece on board
    board[piece.y+piece.shape_alt[0]][piece.x+piece.shape_alt[1]] = 1;
    board[piece.y+piece.shape_alt[2]][piece.x+piece.shape_alt[3]] = 1;
    board[piece.y+piece.shape_alt[4]][piece.x+piece.shape_alt[5]] = 1;
    board[piece.y+piece.shape_alt[6]][piece.x+piece.shape_alt[7]] = 1;

    time_t s = clock();
    // ---------------------------- find clears ----------------------------
    for(int i=piece.y; i<piece.y+4; i++){
        if(i >= 24) break;
        clear++;
        to_clear[i] = true;
        for(int j=0; j<10; j++){
            if(board[i][j] == 0){
                clear--;
                to_clear[i] = false;
                break;
            }
        }
    }
    times[0] += (double)(clock() - s) / CLOCKS_PER_SEC*1000;

    if(isT){
        tclear = clear;
    }
    
    combo += clear > 0;
    b2b += (clear >= 4 || isT);

    time_t s2 = clock();
    // ---------------------------- find max depths ----------------------------
    vector<int> m(10, 24);
    for(int i=0; i<10; i++){
        for(int j=0; j<24; j++){
            if(board[j][i] != 0){
                m[i] = j;
                break;
            }
        }
    }
    times[1] += (double)(clock() - s2) / CLOCKS_PER_SEC*1000;

    // ---------------------------- find t spin setup ----------------------------
    int tsetup = 0;


    time_t s3 = clock();
    bool tVis[28][14][4] = {false};
    deque<int> queue;
    deque<int> actionQueue;

    Piece dummyTPiece(1, 4, 0, 0);
    times[2] += (double)(clock() - s3) / CLOCKS_PER_SEC*1000;

    // std::cout<<"init queue"<<std::endl;

    time_t s4 = clock();
    for(int i=0; i<4; i++){
        for(int j=-2; j<10; j++){
            dummyTPiece.setPosition(j, 0);
            if(checkCollision(dummyTPiece, board)){
                continue;
            }

            if (i == 0) dummyTPiece.setPosition(j, min(m[j], min(m[j+1], m[j+2])-2));
            else if (i == 2) dummyTPiece.setPosition(j, min(m[j], min(m[j+1]+1, m[j+2])-2));
            else if (i == 1) dummyTPiece.setPosition(j, min(m[j], m[j+1]+1)-2);
            else dummyTPiece.setPosition(j, min(m[j+2], m[j+1]+1)-2);

            queue.push_back(positionToBitString(dummyTPiece.x, dummyTPiece.y, dummyTPiece.rotation));
            actionQueue.push_back(-1);
        }
        dummyTPiece.rotate();
    }
    times[3] += (double)(clock() - s4) / CLOCKS_PER_SEC*1000;

    // std::cout<<"queue size: "<<queue.size()<<std::endl;

    time_t s5 = clock();
    while(queue.size() > 0){
        time_t s7 = clock();
        int current = queue.front();
        queue.pop_front();
        int currentAction = actionQueue.front();
        actionQueue.pop_front();

        int y = (current & 0b111111) - 4;
        int x = ((current >> 6) & 0b111111) - 4;
        int rot = ((current >> 12) & 0b1111);

        times[6] += (double)(clock() - s7) / CLOCKS_PER_SEC*1000;

        if (tVis[x+3][y+3][rot]){
            continue;
        }
        time_t s11 = clock();

        tVis[x+3][y+3][rot] = true;

        dummyTPiece.setPositionRotation(x, y, rot);

        if(!checkCollision(dummyTPiece, board, 0, 1)){
            queue.push_back(positionToBitString(dummyTPiece.x, dummyTPiece.y+1, dummyTPiece.rotation));
            actionQueue.push_back(SOFT_DROP);
            times[10] += (double)(clock() - s11) / CLOCKS_PER_SEC*1000;
            continue;
        }
        times[10] += (double)(clock() - s11) / CLOCKS_PER_SEC*1000;
        
        time_t s8 = clock();
        if(!checkCollision(dummyTPiece, board, 1, 0) && !tVis[dummyTPiece.x+4][dummyTPiece.y+3][dummyTPiece.rotation]){
            queue.push_back(positionToBitString(dummyTPiece.x+1, dummyTPiece.y, dummyTPiece.rotation));
            actionQueue.push_back(MOVE_RIGHT);
        }

        if(!checkCollision(dummyTPiece, board, -1, 0) && !tVis[dummyTPiece.x+2][dummyTPiece.y+3][dummyTPiece.rotation]){
            queue.push_back(positionToBitString(dummyTPiece.x-1, dummyTPiece.y, dummyTPiece.rotation));
            actionQueue.push_back(MOVE_LEFT);
        }
        times[7] += (double)(clock() - s8) / CLOCKS_PER_SEC*1000;
        time_t s9 = clock();
        if(rotate(dummyTPiece, board)){
            if(!tVis[dummyTPiece.x+3][dummyTPiece.y+3][(rot+1)&3]){
                queue.push_back(positionToBitString(dummyTPiece.x, dummyTPiece.y, dummyTPiece.rotation));
                actionQueue.push_back(ROTATE_CLOCKWISE);
            }
        }

        dummyTPiece.setPositionRotation(x, y, rot);

        if(rotate(dummyTPiece, board, true)){
            if(!tVis[dummyTPiece.x+3][dummyTPiece.y+3][(rot+3)&3]){
                queue.push_back(positionToBitString(dummyTPiece.x, dummyTPiece.y, dummyTPiece.rotation));
                actionQueue.push_back(ROTATE_COUNTER);
            }
        }

        dummyTPiece.setPositionRotation(x, y, rot);
        times[8] += (double)(clock() - s9) / CLOCKS_PER_SEC*1000;


        time_t s10 = clock();
        if(!isTSpin(dummyTPiece, board, currentAction)){
            continue;
        }
        int hasTSpinSetup = 3;
        for(int j=dummyTPiece.y; j<dummyTPiece.y+3; j++){
            if(j >= 24){
                break;
            }

            for(int i=0; i<10; i++){
                if(board[j][i] == 0 && ((i-dummyTPiece.x) < 0 || (i-dummyTPiece.x) >= 3 || dummyTPiece.shape[j-dummyTPiece.y][i-dummyTPiece.x] == 0)){
                    hasTSpinSetup--;
                    break;
                }
            }
        }

        tsetup = max(tsetup, hasTSpinSetup);
        times[9] += (double)(clock() - s10) / CLOCKS_PER_SEC*1000;
    }

    times[4] += (double)(clock() - s5) / CLOCKS_PER_SEC*1000;

    int maxHeight = 0;
    int holes = 0;
    int caves = 0;
    int hangs = 0;
    int bumpiness = 0;
    int well = 0;
    int well2 = 0;

    // std::cout<<"find well, hangs, holes, caves, bumpiness, maxHeight"<<std::endl;
    
    // for(int i = 0; i < 10; i++){
    //     std::cout<<"m["<<i<<"]: "<<m[i]<<std::endl;
    // }

    // recalc m but with clears
    for(int i=0; i<10; i++){
        for(int j=m[i]; j<24; j++){
            if(board[j][i] != 0 && !to_clear[j]){
                m[i] = j;
                break;
            }
        }
    }

    time_t s6 = clock();
    for(int i=0; i<10; i++){
        //find well - 1x4 hole for tetris setup
        // std::cout<<"pre well"<<std::endl;
        board[0][0] = 0;
        if (m[i] >= 4 && (i <= 0 || m[i] - m[i-1] >= 4) && (i >= 9 || m[i] - m[i+1] >= 4)){
            
            bool iswell = true;
            int end = m[i]-4;
            for(int j = m[i]-1; j >= end; j--){
                if(j < 4) {
                    iswell = false;
                    break;
                }
                if(to_clear[j]){
                    end -= 1;
                    continue;   
                }
                for(int k = 0; k < 10; k++){
                    if(k != i && board[j][k] == 0){
                        iswell = false;
                        break;
                    }
                }
            }
            if (iswell){
                well2 = well || well2;
                well = well==0;
            }
        }

        // std::cout<<"well"<<std::endl;

        if(m[i] == 24) continue;

        //find hangs, holes, caves
        for(int j=m[i]+1; j<24; j++){
            if(board[j][i] != 0 || to_clear[j]) continue;

            // check if hang
            if((i != 0 && m[i-1] > j) || (i != 9 && m[i+1] > j)){
                hangs++;
                continue;
            }

            // cave if above or below is open
            if((j != 23 && board[j+1][i] == 0) || (j != 0 && board[j-1][i] == 0)){
                caves++;
                continue;
            }
            holes++;
        }


        if(i != 0) bumpiness += abs(m[i] - m[i-1]);
        maxHeight = max(maxHeight, 24-m[i]);
    }

    times[5] += (double)(clock() - s6) / CLOCKS_PER_SEC*1000;

    maxHeight -= clear;

    int allclear = maxHeight == 0;

    // wipe piece from board
    board[piece.y+piece.shape_alt[0]][piece.x+piece.shape_alt[1]] = 0;
    board[piece.y+piece.shape_alt[2]][piece.x+piece.shape_alt[3]] = 0;
    board[piece.y+piece.shape_alt[4]][piece.x+piece.shape_alt[5]] = 0;
    board[piece.y+piece.shape_alt[6]][piece.x+piece.shape_alt[7]] = 0;

    vector<int> fitnessValue = {
        bumpiness, bumpiness*bumpiness, 
        holes, holes*holes, 
        caves, caves*caves, 
        hangs, hangs*hangs, 
        clear == 1, clear == 2, clear == 3, clear == 4, 
        tclear == 1, tclear == 2, tclear == 3, 
        tsetup == 1, tsetup == 2, tsetup == 3, 
        well, well2, 
        allclear,
        maxHeight, maxHeight*maxHeight,
        combo == 1, combo == 2, combo == 3, combo == 4, combo >= 5,
        b2b == 1, b2b == 2, b2b >= 3,
        (!isT && piece.shapeNum == 1)
    };

    // std::cout<<"board: "<<std::endl;
    // for(int i = 0; i < 24; i++){
    //     for(int j = 0; j < 10; j++){
    //         std::cout<<board[i][j]<<" ";
    //     }
    //     std::cout<<std::endl;
    // }


    // std::cout<<"tvis: "<<std::endl;
    // for(int i = 0; i < 28; i++){
    //     for(int j = 0; j < 14; j++){
    //         for(int k = 0; k < 4; k++){
    //             std::cout<<tVis[i][j][k]<<" ";
    //         }
    //         std::cout<<std::endl;
    //     }
    //     std::cout<<std::endl;
    // }


    // cout<<"fitnessValue: ";
    // if(true){
    //     cout<<"bumpiness: "<<bumpiness<<", "<<bumpiness*bumpiness<<endl;
    //     cout<<"holes: "<<holes<<", "<<holes*holes<<endl;
    //     cout<<"caves: "<<caves<<", "<<caves*caves<<endl;
    //     cout<<"hangs: "<<hangs<<", "<<hangs*hangs<<endl;
    //     cout<<"clear: "<<clear<<", "<<tclear<<endl;
    //     cout<<"tsetup: "<<tsetup<<endl;
    //     cout<<"well: "<<well<<", "<<well2<<endl;
    //     cout<<"allclear: "<<allclear<<endl;
    //     cout<<"maxHeight: "<<maxHeight<<", "<<maxHeight*maxHeight<<endl;
    // }
    // cout<<endl;

    return fitness(genes, fitnessValue);
}

double timeEval = 0;

vector<int> getBestAction(vector<vector<int>>& board, Piece& piece, vector<int>& genes, int combo, int b2b){
    /***
     * :param: board: 2d vector
     * :param: piece: Piece
     * :param: evalVector: vector
     * :return: vector
     */

    // unordered_map<int, int> parent;
    // unordered_set<int> vis;
    int parent[28][14][4] = {0};
    bool vis[28][14][4] = {false};
    deque<int> queue;
    deque<int> actionQueue;
    deque<int> parentQueue;

    int maxEvalScore = -1000000000;
    int maxEvalParent = -1;

    int initialPositionRotation[3] = {piece.x, piece.y, piece.rotation};
    
    parent[initialPositionRotation[1]+3][initialPositionRotation[0]+3][initialPositionRotation[2]] = -1;
    parent[initialPositionRotation[1]+3][initialPositionRotation[0]+3][(initialPositionRotation[2]+3)%4] = positionActionToBitString(initialPositionRotation[0], initialPositionRotation[1], initialPositionRotation[2], ROTATE_COUNTER);
    parent[initialPositionRotation[1]+3][initialPositionRotation[0]+3][(initialPositionRotation[2]+1)%4] = positionActionToBitString(initialPositionRotation[0], initialPositionRotation[1], initialPositionRotation[2], ROTATE_CLOCKWISE);
    parent[initialPositionRotation[1]+3][initialPositionRotation[0]+3][(initialPositionRotation[2]+2)%4] = positionActionToBitString(initialPositionRotation[0], initialPositionRotation[1], (initialPositionRotation[2]+1)%4, ROTATE_CLOCKWISE);

    for(int i=0; i<4; i++){
        for(int j=-2; j<10; j++){
            piece.setPosition(j, initialPositionRotation[1]);
            if(checkCollision(piece, board)){
                continue;
            }

            int bitstring = positionToBitString(j, piece.y, piece.rotation);
            if(j < initialPositionRotation[0]){
                parent[piece.y+3][j+3][piece.rotation] = positionActionToBitString(j+1, piece.y, piece.rotation, MOVE_LEFT);
            } else if(j > initialPositionRotation[0]){
                parent[piece.y+3][j+3][piece.rotation] = positionActionToBitString(j-1, piece.y, piece.rotation, MOVE_RIGHT);
            }

            int originalY = piece.y;
            fastDrop(piece, board);

            for(int row=originalY; row<piece.y; row++){
                parent[row+4][j+3][piece.rotation] = positionActionToBitString(j, row, piece.rotation, SOFT_DROP);
                vis[row+3][j+3][piece.rotation] = true;
                // parent[positionToBitString(j, row+1, piece.rotation)] = positionActionToBitString(j, row, piece.rotation, SOFT_DROP);
                // vis.insert(positionToBitString(j, row, piece.rotation));
            }

            queue.push_back(positionToBitString(piece.x, piece.y, piece.rotation));
            parentQueue.push_back(-1);
            actionQueue.push_back(-1);
        }
        piece.rotate();
    }

    while(queue.size() > 0){
        int current = queue.front();
        queue.pop_front();
        int currentAction = actionQueue.front();
        actionQueue.pop_front();
        int parentPosition = parentQueue.front();
        parentQueue.pop_front();

        int y = (current & 0b111111) - 4;
        int x = ((current >> 6) & 0b111111) - 4;
        int rot = ((current >> 12) & 0b1111);

        if(vis[y+3][x+3][rot]){
            continue;
        }

        vis[y+3][x+3][rot] = true;
        // if(parentPosition != -1) parent[current] = parentPosition | (currentAction << 16);
        if(parentPosition != -1) parent[y+3][x+3][rot] = parentPosition | (currentAction << 16);


        piece.setPositionRotation(x, y, rot);

        // ---------------------- fast drop ----------------------

        if(!checkCollision(piece, board, 0, 1)){
            queue.push_back(positionToBitString(x, y+1, rot));
            actionQueue.push_back(SOFT_DROP);
            parentQueue.push_back(current);
            continue;
        }
        
        // ---------------------- check adj positions ----------------------
        if(!checkCollision(piece, board, 1, 0) && vis[y+3][x+4][rot] == false){
            queue.push_back(positionToBitString(x+1, y, rot));
            actionQueue.push_back(MOVE_RIGHT);
            parentQueue.push_back(current);
        }
        if(!checkCollision(piece, board, -1, 0) && vis[y+3][x+2][rot] == false){
            queue.push_back(positionToBitString(x-1, y, rot));
            actionQueue.push_back(MOVE_LEFT);
            parentQueue.push_back(current);
        }
        if(rotate(piece, board)){
            if(!vis[piece.y+3][piece.x+3][piece.rotation]){
                queue.push_back(positionToBitString(piece.x, piece.y, piece.rotation));
                actionQueue.push_back(ROTATE_CLOCKWISE);
                parentQueue.push_back(current);
            }
        }
        piece.setPositionRotation(x, y, rot);
        if(rotate(piece, board, true)){
            if(!vis[piece.y+3][piece.x+3][piece.rotation]){
                queue.push_back(positionToBitString(piece.x, piece.y, piece.rotation));
                actionQueue.push_back(ROTATE_COUNTER);
                parentQueue.push_back(current);
            }
        }
        piece.setPositionRotation(x, y, rot);

        // ---------------------- evaluate position (guarenteed not soft-dropable) ----------------
        time_t start = clock();
        int eval = evaluateBoard(genes, board, piece, currentAction, combo, b2b);
        timeEval += (double)(clock() - start) / CLOCKS_PER_SEC*1000;

        if(eval > maxEvalScore){
            maxEvalScore = eval;
            maxEvalParent = current;
        }
    }

    // for(int i = 0; i < 28; i++){
    //     for(int j = 0; j < 14; j++){
    //         cout<<parent[i][j][0]<<" ";
    //     }
    //     cout<<endl;
    // }
    // for(int i = 0; i < 28; i++){
    //     for(int j = 0; j < 14; j++){
    //         cout<<parent[i][j][1]<<" ";
    //     }
    //     cout<<endl;
    // }
    // for(int i = 0; i < 28; i++){
    //     for(int j = 0; j < 14; j++){
    //         cout<<parent[i][j][2]<<" ";
    //     }
    //     cout<<endl;
    // }
    // for(int i = 0; i < 28; i++){
    //     for(int j = 0; j < 14; j++){
    //         cout<<parent[i][j][3]<<" ";
    //     }
    //     cout<<endl;
    // }

    vector<int> path = vector<int>();
    // std::cout<<(maxEvalParent >> 6 & 0b111111) - 4<<" "<<(maxEvalParent & 0b111111) - 4<<" "<<(maxEvalParent >> 12 & 0b1111)<<std::endl;
    // std::cout<<parent[(maxEvalParent & 0b111111) - 4][(maxEvalParent >> 6 & 0b111111) - 4][(maxEvalParent >> 12 & 0b1111)]<<std::endl;
    try{
        while(maxEvalParent != -1){
            // std::cout<<"maxEvalParent: "<<maxEvalParent<<std::endl;
            path.push_back(parent[(maxEvalParent & 0b111111) - 1][(maxEvalParent >> 6 & 0b111111) - 1][(maxEvalParent >> 12 & 0b1111)] >> 16);
            maxEvalParent = parent[(maxEvalParent & 0b111111) - 1][(maxEvalParent >> 6 & 0b111111) - 1][(maxEvalParent >> 12 & 0b1111)];
        }
    } catch (const std::out_of_range& e){
        std::cout<<"AIController.getBestAction() : out_of_range"<<std::endl;
        std::cout<<"Piece: "<<piece.shapeNum<<std::endl;
        std::cout<<path.size()<<std::endl;
        for(int i=0; i<path.size(); i++){
            std::cout<<path[i]<<std::endl;
        }
        std::cout<<board.size()<<std::endl;
        for(int i=0; i<board.size(); i++){
            for(int j=0; j<board[0].size(); j++){
                std::cout<<board[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
        std::cout<<"Init position: "<<initialPositionRotation[0]<<" "<<initialPositionRotation[1]<<" "<<initialPositionRotation[2]<<std::endl;
        // for(int i=0; i<28; i++){.

    }

    //reverse 
    reverse(path.begin(), path.end());
    path.push_back(maxEvalScore);

    piece.setPositionRotation(initialPositionRotation[0], initialPositionRotation[1], initialPositionRotation[2]);

    return path;
}

/*
vector<int> getBestAction(vector<vector<int>>& board, Piece& piece, Piece& piece_hold, std::vector<Piece>& piece_queue, int piece_queue_index, vector<int> evalVector, int depth=0, bool holdUsed=false){
    // unordered_map<int, int> parent;
    // unordered_set<int> vis;
    int parent[28][14][4] = {0};
    bool vis[28][14][4] = {false};
    deque<int> queue;
    deque<int> actionQueue;
    deque<int> parentQueue;

    int maxEvalScore = -1000000000;
    int maxEvalParent = -1;
    vector<int> depthPath = vector<int>();
    bool hold = false;

    int initialPositionRotation[3] = {piece.x, piece.y, piece.rotation};
    
    parent[initialPositionRotation[1]+3][initialPositionRotation[0]+3][initialPositionRotation[2]] = -1;
    parent[initialPositionRotation[1]+3][initialPositionRotation[0]+3][(initialPositionRotation[2]+3)%4] = positionActionToBitString(initialPositionRotation[0], initialPositionRotation[1], initialPositionRotation[2], ROTATE_COUNTER);
    parent[initialPositionRotation[1]+3][initialPositionRotation[0]+3][(initialPositionRotation[2]+1)%4] = positionActionToBitString(initialPositionRotation[0], initialPositionRotation[1], initialPositionRotation[2], ROTATE_CLOCKWISE);
    parent[initialPositionRotation[1]+3][initialPositionRotation[0]+3][(initialPositionRotation[2]+2)%4] = positionActionToBitString(initialPositionRotation[0], initialPositionRotation[1], (initialPositionRotation[2]+1)%4, ROTATE_CLOCKWISE);

    for(int i=0; i<4; i++){
        for(int j=-2; j<10; j++){
            piece.setPosition(j, initialPositionRotation[1]);
            if(checkCollision(piece, board)){
                continue;
            }

            int bitstring = positionToBitString(j, piece.y, piece.rotation);
            if(j < initialPositionRotation[0]){
                parent[piece.y+3][j+3][piece.rotation] = positionActionToBitString(j+1, piece.y, piece.rotation, MOVE_LEFT);
            } else if(j > initialPositionRotation[0]){
                parent[piece.y+3][j+3][piece.rotation] = positionActionToBitString(j-1, piece.y, piece.rotation, MOVE_RIGHT);
            }

            int originalY = piece.y;
            fastDrop(piece, board);

            for(int row=originalY; row<piece.y; row++){
                parent[row+4][j+3][piece.rotation] = positionActionToBitString(j, row, piece.rotation, SOFT_DROP);
                vis[row+3][j+3][piece.rotation] = true;
                // parent[positionToBitString(j, row+1, piece.rotation)] = positionActionToBitString(j, row, piece.rotation, SOFT_DROP);
                // vis.insert(positionToBitString(j, row, piece.rotation));
            }

            queue.push_back(positionToBitString(piece.x, piece.y, piece.rotation));
            parentQueue.push_back(-1);
            actionQueue.push_back(-1);
        }
        piece.rotate();
    }

    while(queue.size() > 0){
        int current = queue.front();
        queue.pop_front();
        int currentAction = actionQueue.front();
        actionQueue.pop_front();
        int parentPosition = parentQueue.front();
        parentQueue.pop_front();

        int y = (current & 0b111111) - 4;
        int x = ((current >> 6) & 0b111111) - 4;
        int rot = ((current >> 12) & 0b1111);

        if(vis[y+3][x+3][rot]){
            continue;
        }

        vis[y+3][x+3][rot] = true;
        // if(parentPosition != -1) parent[current] = parentPosition | (currentAction << 16);
        if(parentPosition != -1) parent[y+3][x+3][rot] = parentPosition | (currentAction << 16);


        piece.setPositionRotation(x, y, rot);

        // ---------------------- fast drop ----------------------

        if(!checkCollision(piece, board, 0, 1)){
            queue.push_back(positionToBitString(x, y+1, rot));
            actionQueue.push_back(SOFT_DROP);
            parentQueue.push_back(current);
            continue;
        }
        
        // ---------------------- check adj positions ----------------------
        if(!checkCollision(piece, board, 1, 0) && vis[y+3][x+4][rot] == false){
            queue.push_back(positionToBitString(x+1, y, rot));
            actionQueue.push_back(MOVE_RIGHT);
            parentQueue.push_back(current);
        }
        if(!checkCollision(piece, board, -1, 0) && vis[y+3][x+2][rot] == false){
            queue.push_back(positionToBitString(x-1, y, rot));
            actionQueue.push_back(MOVE_LEFT);
            parentQueue.push_back(current);
        }
        if(rotate(piece, board)){
            if(!vis[piece.y+3][piece.x+3][piece.rotation]){
                queue.push_back(positionToBitString(piece.x, piece.y, piece.rotation));
                actionQueue.push_back(ROTATE_CLOCKWISE);
                parentQueue.push_back(current);
            }
        }
        piece.setPositionRotation(x, y, rot);
        if(rotate(piece, board, true)){
            if(!vis[piece.y+3][piece.x+3][piece.rotation]){
                queue.push_back(positionToBitString(piece.x, piece.y, piece.rotation));
                actionQueue.push_back(ROTATE_COUNTER);
                parentQueue.push_back(current);
            }
        }
        piece.setPositionRotation(x, y, rot);

        // ---------------------- evaluate position (guarenteed not soft-dropable) ----------------
        // int eval = evaluateBoard(evalVector, board, piece, currentAction);
        int eval;
        bool isOriginal = piece_queue_index == -1;

        if (!isOriginal){
            swapPiece(&piece, &piece_queue[piece_queue_index]);
        }

        if(depth == 0){

            eval = evaluateBoard(evalVector, board, piece, currentAction);

            if (!isOriginal){
                swapPiece(&piece, &piece_queue[piece_queue_index]);
            }

        } else { // search deeper
            //place piece
            board[piece.y+piece.shape_alt[0]][piece.x+piece.shape_alt[1]] = 1;
            board[piece.y+piece.shape_alt[2]][piece.x+piece.shape_alt[3]] = 1;
            board[piece.y+piece.shape_alt[4]][piece.x+piece.shape_alt[5]] = 1;
            board[piece.y+piece.shape_alt[6]][piece.x+piece.shape_alt[7]] = 1;

            //find best action for next piece
            vector<int> nextPiecePath = getBestAction(board, piece_queue[piece_queue_index+1], piece_hold, piece_queue, piece_queue_index+1, evalVector, depth-1, false);

            //test hold
            if(piece_hold.shapeNum != -1 && !holdUsed){
                swapPiece(&piece, &piece_hold);
                vector<int> holdPath = getBestAction(board, piece, piece_queue[piece_queue_index], piece_queue, piece_queue_index, evalVector, depth-1, true);

                if(holdPath[0] > nextPiecePath[0]){
                    nextPiecePath = holdPath;
                    hold = true;
                } else {
                    swapPiece(&piece, &piece_hold);
                }
            }
        }

        if(eval > maxEvalScore){
            maxEvalScore = eval;
            maxEvalParent = current;
        }
    }

    vector<int> path;
    try{
        // while(maxEval[1] != -1){
        //     if(maxEval[1] == 0) {
        //         throw out_of_range("out of range");
        //     }
        //     path.push_back(parent[(maxEval[1] & 0b111111) - 1][(maxEval[1] >> 6 & 0b111111) - 1][(maxEval[1] >> 12 & 0b1111)] >> 16);
        //     maxEval[1] = parent[(maxEval[1] & 0b111111) - 1][(maxEval[1] >> 6 & 0b111111) - 1][(maxEval[1] >> 12 & 0b1111)];
        // }
        while(maxEvalParent != -1){
            path.push_back(parent[(maxEvalParent & 0b111111) - 4][(maxEvalParent >> 6 & 0b111111) - 4][(maxEvalParent >> 12 & 0b1111)] >> 16);
            maxEvalParent = parent[(maxEvalParent & 0b111111) - 4][(maxEvalParent >> 6 & 0b111111) - 4][(maxEvalParent >> 12 & 0b1111)];
        }
    } catch (const std::out_of_range& e){
        std::cout<<"AIController.getBestAction() : out_of_range"<<std::endl;
        std::cout<<"Piece: "<<piece.shapeNum<<std::endl;
        std::cout<<path.size()<<std::endl;
        for(int i=0; i<path.size(); i++){
            std::cout<<path[i]<<std::endl;
        }
        std::cout<<board.size()<<std::endl;
        for(int i=0; i<board.size(); i++){
            for(int j=0; j<board[0].size(); j++){
                std::cout<<board[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
        std::cout<<"Init position: "<<initialPositionRotation[0]<<" "<<initialPositionRotation[1]<<" "<<initialPositionRotation[2]<<std::endl;
        // for(int i=0; i<28; i++){.

    }

    // ( [score] [n] [to_hold] [action1] [action2] ... [actionn] ) x depth
    path.push_back(0);
    path.push_back(path.size()-1);
    path.push_back(maxEvalScore);

    //reverse 
    reverse(path.begin(), path.end());

    if(depth == 0){
        return path;
    }

    path.insert(path.end(), depthPath.begin(), depthPath.end());

    piece.setPositionRotation(initialPositionRotation[0], initialPositionRotation[1], initialPositionRotation[2]);

    return path;
}
*/

// int main(){
//     Piece piece = Piece(1, 4, 0, 0);
//     std::vector<std::vector<int>> board = {
//     //   0, 1, 2, 3, 4, 5, 6, 7, 8, 9
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 1
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 2
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 3
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 4
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 5
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 6
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 7
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 8
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 9
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 10
//         {0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // 11
//         {0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // 12
//         {0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // 13
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // 14
//         {1, 0, 0, 0, 0, 0, 1, 1, 1, 4}, // 15
//         {1, 0, 0, 0, 0, 0, 0, 1, 1, 1}, // 16
//         {1, 1, 0, 0, 0, 0, 1, 1, 1, 2}, // 17
//         {1, 1, 1, 0, 0, 0, 1, 2, 4, 2}, // 18
//         {1, 1, 0, 0, 0, 3, 1, 1, 1, 3}, // 19
//         {5, 1, 1, 0, 4, 3, 1, 3, 1, 5}, // 20
//         {1, 0, 1, 2, 2, 2, 2, 2, 1, 1}, // 21
//         {1, 0, 1, 7, 1, 2, 1, 6, 1, 1}, // 22
//         {1, 0, 1, 1, 1, 2, 1, 7, 1, 1}  // 23
//     };
//     // PieceRandomizer pr = PieceRandomizer();
//     // Piece piece = pr.nextPiece();
    
//     if (checkCollision(piece, board)){
//         std::cout<<"Collision"<<std::endl;
//         return 0;
//     }

//     // std::vector<int> genes = get_default();
//     // evaluateBoard(genes, board, piece, 4);


//     // Piece testpiece = Piece(1, 5, 18, 0);
//     // bool res = rotate(testpiece, board);

//     // std::cout<<res<<" "<<testpiece.x<<" "<<testpiece.y<<" "<<testpiece.rotation<<std::endl;

//     std::vector<int> genes = get_default();
//     clock_t start = clock();
//     for(int i = 0; i < 100; i++){
//         vector<int> path = getBestAction(board, piece, genes);
//     }
//     clock_t end = clock();
//     double elapsed_ms = double(end - start) / CLOCKS_PER_SEC * 1000;
//     std::cout<<"getBestAction x20: "<<elapsed_ms<<"ms"<<std::endl;
//     std::cout<<"timeEval: "<<timeEval<<std::endl;

//     for(int i = 0; i < (int)(sizeof(times) / sizeof(double)); i++){
//         std::cout<<"time "<<i<<": "<<times[i]<<std::endl;
//     }

//     return 0;
// }

