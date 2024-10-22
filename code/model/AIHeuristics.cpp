
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

#include <cstdio>
#include <ctime>

#include "AIModel.h"
#include "../tetris_lib/Piece.h"
#include "../tetris_lib/Collision.h"
#include "../tetris_lib/constants.h"
// #include "AIHeuristics.h"

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

// double times[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
double time_set_inital = 0;
double time_check_adj = 0;
double time_rotate = 0;
double time_eval = 0;
double time_pop = 0;
double time_calc = 0;

int eval_count = 0;
int* eval_count_depth = new int[5]{0,0,0,0,0};

int evaluateBoard(vector<int>& genes, int board[24][10], Piece& piece, int lastAction, int* combo, int* b2b, int pending){
    eval_count++;
    int clear = 0;
    int tclear = 0;
    bool isT = isTSpin(piece, board, lastAction);
    // if(isT) cout<<"T-Spin"<<endl;

    bool to_clear[24] = {false};

    // put piece on board
    board[piece.y+piece.shape_alt[0]][piece.x+piece.shape_alt[1]] = 1;
    board[piece.y+piece.shape_alt[2]][piece.x+piece.shape_alt[3]] = 1;
    board[piece.y+piece.shape_alt[4]][piece.x+piece.shape_alt[5]] = 1;
    board[piece.y+piece.shape_alt[6]][piece.x+piece.shape_alt[7]] = 1;

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
    if(isT) tclear = clear;
    // *combo += clear > 0;
    if(clear>0){
        *combo += 1;
        if(clear == 4 || isT){
            *b2b += 1;
        } else {
            *b2b = 0;
        }
    } else {
        *combo = 0;
        *b2b = 0;
    }

    // ---------------------------- find max depths ----------------------------
    int* m = new int[10];
    // std::vector<int> m(10, 0);
    int low_depth = 0;
    int low_depth_col = 0;

    // ---------------------------- find t spin setup ----------------------------
    int tsetup = 0;

    int maxHeight = 0;
    int holes = 0;
    int caves = 0;
    int hangs = 0;
    int bumpiness = 0;
    int well = 0;

    // recalc m but with clears
    for(int i=0; i<10; i++){
        for(int j=0; j<=24; j++){
            if(j==24 || (board[j][i] != 0 && !to_clear[j])){
                m[i] = j;
                if (j > low_depth){
                    low_depth = j;
                    low_depth_col = i;
                }
                break;
            }
        }
    }

    //calc well
    bool tobreak = false;
    for(int j=m[low_depth_col]-1; j >= 0; j--){
        if(to_clear[j]) continue;
        if(board[j][low_depth_col] != 0) {
            assert(false);
        }

        well++;
        for(int i=0; i<10; i++){
            if(i == low_depth_col) continue;
            if(board[j][i] == 0){
                well--;
                tobreak = true;
                break;
            }
        }   
        if(tobreak) break;
    }

    //calc bumpiness, hangs, holes, caves, maxHeight
    for(int i=0; i<10; i++){

        if(m[i] == 24) continue;

        //find hangs, holes, caves
        for(int j=m[i]+1; j<24; j++){
            if(board[j][i] != 0 || to_clear[j]) continue;

            // check if hang
            // if((i != 0 && m[i-1] > j) || (i != 9 && m[i+1] > j)){
            if((i > 1 && m[i-1] > j && m[i-2] > j) || (i < 8 && m[i+1] > j && m[i+2] > j)){
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

    // time_calc += (double)(clock() - time_calc_start);

    delete[] m;

    // maxHeight -= clear;

    int allclear = maxHeight == 0;

    maxHeight += pending;


    // wipe piece from board
    board[piece.y+piece.shape_alt[0]][piece.x+piece.shape_alt[1]] = 0;
    board[piece.y+piece.shape_alt[2]][piece.x+piece.shape_alt[3]] = 0;
    board[piece.y+piece.shape_alt[4]][piece.x+piece.shape_alt[5]] = 0;
    board[piece.y+piece.shape_alt[6]][piece.x+piece.shape_alt[7]] = 0;

    // if(maxHeight >= 20){ // if piece is above the board
    //     return -100000000;
    // }

    int fit = 0;

    fit += genes[0] * bumpiness;
    fit += genes[1] * bumpiness*bumpiness;
    fit += genes[2] * holes;
    fit += genes[3] * holes*holes;
    fit += genes[4] * caves;
    fit += genes[5] * caves*caves;
    fit += genes[6] * hangs;
    fit += genes[7] * hangs*hangs;
    fit += genes[8] * (clear == 1);
    fit += genes[9] * (clear == 2);
    fit += genes[10] * (clear == 3);
    fit += genes[11] * (clear == 4);
    fit += genes[12] * (tsetup == 1);
    fit += genes[13] * (tsetup == 2);
    fit += genes[14] * (tsetup == 3);
    fit += genes[15] * (tclear == 1);
    fit += genes[16] * (tclear == 2);
    fit += genes[17] * (tclear == 3);
    fit += genes[18] * allclear;
    fit += genes[19] * maxHeight;
    fit += genes[20] * maxHeight*maxHeight;
    fit += genes[21] * (*combo == 1);
    fit += genes[22] * (*combo == 2);
    fit += genes[23] * (*combo == 3);
    fit += genes[24] * (*combo == 4);
    fit += genes[25] * (*combo >= 5);
    fit += genes[26] * (*b2b == 1);
    fit += genes[27] * (*b2b == 2);
    fit += genes[28] * (*b2b >= 3);
    fit += genes[29] * (!isT && piece.shapeNum == 1);
    fit += genes[30] * well;
    fit += genes[31] * (well == 2);
    fit += genes[32] * (well == 3);
    fit += genes[33] * (well == 4);
    fit += genes[34] * (well == 5);
    fit += genes[35] * (well == 6);
    fit += genes[36] * (well == 7);
    fit += genes[37] * (well == 8);
    fit += genes[38] * (well == 9);
    fit += genes[39] * (std::max(0, maxHeight-15));
    fit += genes[40] * (piece.shapeNum == 0 && (piece.rotation == 0 || piece.rotation == 2 || clear == 0) && tsetup == 0);
    
    return fit;
}


int getBestPathlessAction(int board[24][10], Piece* piece_hold, vector<Piece>& piece_queue, int piece_queue_index, int depth, vector<int>& genes, int* combo, int* b2b, bool holdUsed, int pending){
    /***
     * :param: board: 2d vector
     * :param: piece: Piece
     * :param: piece_hold: Piece
     * :param: piece_queue: vector
     * :param: piece_queue_index: int
     * :param: evalVector: vector
     * :param: depth: int
     * :return: int
     * 
     */
    Piece piece = piece_queue[piece_queue_index];


    bool vis[28][14][4] = {false};
    deque<int> queue;
    deque<int> actionQueue;

    int maxEvalScore = -1000000000;
    
    int initialPositionRotation[3] = {piece.x, piece.y, piece.rotation};

    // time_t start = clock();
    for(int i=0; i<4; i++){
        for(int j=-2; j<10; j++){
            piece.setPosition(j, initialPositionRotation[1]);
            if(checkCollision(piece, board)){
                continue;
            }

            int originalY = piece.y;
            fastDrop(piece, board);

            for(int row=originalY; row<piece.y; row++){
                vis[row+3][j+3][piece.rotation] = true;
            }

            queue.push_back(positionToBitString(piece.x, piece.y, piece.rotation));
            actionQueue.push_back(-1);
        }
        piece.rotate();
    }
    // time_set_inital += (double)(clock() - start);

    while(!queue.empty()){
        // time_t time_pop_start = clock();
        int current = queue.front();
        queue.pop_front();
        int currentAction = actionQueue.front();
        actionQueue.pop_front();

        int y = (current & 0b111111) - 4;
        int x = ((current >> 6) & 0b111111) - 4;
        int rot = ((current >> 12) & 0b1111);
        // time_pop += (double)(clock() - time_pop_start);

        if(vis[y+3][x+3][rot]){
            continue;
        }

        vis[y+3][x+3][rot] = true;

        // time_t time_check_adj_start = clock();
        piece.setPositionRotation(x, y, rot);

        if(!checkCollision(piece, board, 0, 1)){
            queue.push_back(positionToBitString(x, y+1, rot));
            actionQueue.push_back(SOFT_DROP);
            continue;
        }
        
        if(!checkCollision(piece, board, 1, 0) && !vis[y+3][x+4][rot]){
            queue.push_back(positionToBitString(x+1, y, rot));
            actionQueue.push_back(MOVE_RIGHT);
        }

        if(!checkCollision(piece, board, -1, 0) && !vis[y+3][x+2][rot]){
            queue.push_back(positionToBitString(x-1, y, rot));
            actionQueue.push_back(MOVE_LEFT);
        }

        if(rotate(piece, board)){
            if(!vis[piece.y+3][piece.x+3][piece.rotation]){
                queue.push_back(positionToBitString(piece.x, piece.y, piece.rotation));
                actionQueue.push_back(ROTATE_CLOCKWISE);
            }
        }

        piece.setPositionRotation(x, y, rot);

        if(rotate(piece, board, true)){
            if(!vis[piece.y+3][piece.x+3][piece.rotation]){
                queue.push_back(positionToBitString(piece.x, piece.y, piece.rotation));
                actionQueue.push_back(ROTATE_COUNTER);
            }
        }

        piece.setPositionRotation(x, y, rot);

        // time_check_adj += (double)(clock() - time_check_adj_start);

        // maxEvalScore = max(maxEvalScore, evaluateBoard(genes, board, piece, currentAction, combo, b2b));
        int old_combo = *combo;
        int old_b2b = *b2b;
        // time_t time_eval_start = clock();
        eval_count_depth[depth]++;
        int currentEvalScore = evaluateBoard(genes, board, piece, currentAction, combo, b2b, pending);
        // time_eval += (double)(clock() - time_eval_start);
        if(depth > 0){

            board[piece.y+piece.shape_alt[0]][piece.x+piece.shape_alt[1]] = 1;
            board[piece.y+piece.shape_alt[2]][piece.x+piece.shape_alt[3]] = 1;
            board[piece.y+piece.shape_alt[4]][piece.x+piece.shape_alt[5]] = 1;
            board[piece.y+piece.shape_alt[6]][piece.x+piece.shape_alt[7]] = 1;

            assert(piece_queue_index+1 < piece_queue.size());
            currentEvalScore += getBestPathlessAction(board, piece_hold, piece_queue, piece_queue_index+1, depth-1, genes, combo, b2b, false, pending);

            board[piece.y+piece.shape_alt[0]][piece.x+piece.shape_alt[1]] = 0;
            board[piece.y+piece.shape_alt[2]][piece.x+piece.shape_alt[3]] = 0;
            board[piece.y+piece.shape_alt[4]][piece.x+piece.shape_alt[5]] = 0;
            board[piece.y+piece.shape_alt[6]][piece.x+piece.shape_alt[7]] = 0;
        }

        maxEvalScore = max(maxEvalScore, currentEvalScore);

        *combo = old_combo;
        *b2b = old_b2b;
    }

    piece_queue[piece_queue_index].setPositionRotation(initialPositionRotation[0], initialPositionRotation[1], initialPositionRotation[2]);

    //swap hold piece and current piece and recurse for hold piece score
    if(holdUsed){
        return maxEvalScore;
    }

    if(piece_hold->shapeNum == -1){
        return max(maxEvalScore, 0);
    }

    swapPiece(piece_hold, &piece_queue[piece_queue_index]);

    int holdEvalScore = getBestPathlessAction(board, &piece, piece_queue, piece_queue_index, depth, genes, combo, b2b, true, pending);

    swapPiece(piece_hold, &piece_queue[piece_queue_index]);

    return max(maxEvalScore, holdEvalScore);
}

vector<int> getBestAction(int board[24][10], Piece& piece, Piece* piece_hold, vector<Piece>& piece_queue, int piece_queue_index, int depth, vector<int>& genes, int combo, int b2b, int pending){
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

    // std::cout<<"init"<<std::endl;
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
            // std::cout<<"fastdrop"<<std::endl;
            fastDrop(piece, board);
            // std::cout<<"fastdrop done"<<std::endl;

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

        // std::cout<<"fastdrop"<<std::endl;
        if(!checkCollision(piece, board, 0, 1)){
            queue.push_back(positionToBitString(x, y+1, rot));
            actionQueue.push_back(SOFT_DROP);
            parentQueue.push_back(current);
            continue;
        }
        
        // ---------------------- check adj positions ----------------------
        // std::cout<<"check adj"<<std::endl;
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

        // std::cout<<"rotate"<<std::endl;
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
        // time_t start = clock();
        int old_combo = combo;
        int old_b2b = b2b;

        eval_count_depth[depth]++;
        int eval = evaluateBoard(genes, board, piece, currentAction, &combo, &b2b, pending);

        if(depth > 0){
            board[piece.y+piece.shape_alt[0]][piece.x+piece.shape_alt[1]] = 1;
            board[piece.y+piece.shape_alt[2]][piece.x+piece.shape_alt[3]] = 1;
            board[piece.y+piece.shape_alt[4]][piece.x+piece.shape_alt[5]] = 1;
            board[piece.y+piece.shape_alt[6]][piece.x+piece.shape_alt[7]] = 1;

            assert(depth <= piece_queue.size());

            // std::cout<<"depth: "<<depth<<std::endl;
            eval += getBestPathlessAction(board, piece_hold, piece_queue, piece_queue_index, depth-1, genes, &combo, &b2b, false, pending);

            board[piece.y+piece.shape_alt[0]][piece.x+piece.shape_alt[1]] = 0;
            board[piece.y+piece.shape_alt[2]][piece.x+piece.shape_alt[3]] = 0;
            board[piece.y+piece.shape_alt[4]][piece.x+piece.shape_alt[5]] = 0;
            board[piece.y+piece.shape_alt[6]][piece.x+piece.shape_alt[7]] = 0;
        }

        if(eval > maxEvalScore){
            maxEvalScore = eval;
            maxEvalParent = current;
        }

        combo = old_combo;
        b2b = old_b2b;
    }

    vector<int> path = vector<int>();

    try{
        while(maxEvalParent != -1){
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
        // std::cout<<board.size()<<std::endl;
        for(int i=0; i<24; i++){
            for(int j=0; j<10; j++){
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

// int main(){

//     std::cout<<"begin"<<std::endl;
//     Piece piece = Piece(3, 4, 0, 0);
//     std::cout<<"piece"<<std::endl;
//     int board[24][10] = {
//                 //   0, 1, 2, 3, 4, 5, 6, 7, 8, 9
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
//         {0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // 14
//         {0, 0, 0, 0, 0, 0, 1, 1, 1, 4}, // 15
//         {0, 0, 0, 0, 2, 1, 1, 1, 1, 1}, // 16
//         {0, 1, 0, 0, 0, 2, 1, 1, 1, 2}, // 17
//         {0, 1, 1, 0, 3, 2, 1, 2, 4, 2}, // 18
//         {1, 1, 1, 1, 2, 3, 0, 1, 1, 3}, // 19
//         {5, 1, 1, 1, 4, 3, 1, 3, 1, 5}, // 20
//         {1, 3, 1, 1, 2, 2, 2, 2, 1, 1}, // 21
//         {1, 0, 1, 1, 1, 2, 1, 6, 1, 1}, // 22
//         {1, 0, 1, 1, 1, 2, 1, 7, 1, 1}  // 23
//     };
//     // std::vector<std::vector<int>> board = {
//     // //   0, 1, 2, 3, 4, 5, 6, 7, 8, 9
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 1
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 2
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 3
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 4
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 5
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 6
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 7
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 8
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 9
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 10
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // 11
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // 12
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // 13
//     //     {0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // 14
//     //     {0, 0, 0, 0, 0, 0, 1, 1, 1, 4}, // 15
//     //     {0, 0, 0, 0, 2, 1, 1, 1, 1, 1}, // 16
//     //     {0, 1, 0, 0, 0, 2, 1, 1, 1, 2}, // 17
//     //     {0, 1, 1, 0, 3, 2, 1, 2, 4, 2}, // 18
//     //     {1, 1, 1, 1, 2, 3, 0, 1, 1, 3}, // 19
//     //     {5, 1, 1, 1, 4, 3, 1, 3, 1, 5}, // 20
//     //     {1, 3, 1, 1, 2, 2, 2, 2, 1, 1}, // 21
//     //     {1, 0, 1, 1, 1, 2, 1, 6, 1, 1}, // 22
//     //     {1, 0, 1, 1, 1, 2, 1, 7, 1, 1}  // 23
//     // };
    
//     if (checkCollision(piece, board)){
//         std::cout<<"Collision"<<std::endl;
//         return 0;
//     }

//     std::cout<<"No Collision"<<std::endl;

//     std::vector<Piece> piece_queue = std::vector<Piece>(5);

//     std::cout<<"piece_queue"<<std::endl;

//     piece_queue[0] = Piece(6, 4, 0, 0);
//     piece_queue[1] = Piece(3, 4, 0, 0);
//     piece_queue[2] = Piece(3, 4, 0, 0);
//     piece_queue[3] = Piece(3, 4, 0, 0);
//     piece_queue[4] = Piece(3, 4, 0, 0);
//     //     Piece(7, 4, 0, 0),
//     //     Piece(3, 4, 0, 0),
//     //     Piece(3, 4, 0, 0),
//     //     Piece(3, 4, 0, 0),
//     //     Piece(3, 4, 0, 0),        
//     // };

//     Piece piece_hold = Piece(1, 4, 0, 0);

//     std::cout<<"piece_queue"<<std::endl;

//     std::vector<int> genes = std::vector<int>{
//         0, 0, // bumpiness
//         0, 0, // holes
//         0, 0, // caves
//         0, 0, // hangs
//         0, 0, 0, 0, // clear
//         0, 0, 0, // tsetup
//         0, 100, 0, // tclear
//         0, //allclear
//         0, 0, // maxHeight
//         0, 0, 0, 0, 0, // combo
//         0, 0, 0, // b2b
//         0, // isT
//         0, 0, 0, 0, 0, 0, 0, 0, 0 // well
//     };

//     clock_t start = clock();
//     for(int i = 0; i < 36; i++){
//         std::vector<int> path = getBestAction(board, piece, &piece_hold, piece_queue, 0, 2, genes, 0, 0);
//     }
//     double duration = (double)(clock() - start) / CLOCKS_PER_SEC*1000;
//     std::vector<int> path = getBestAction(board, piece, &piece_hold, piece_queue, 0, 2, genes, 0, 0);
//     std::cout<<"Time: "<<duration<<std::endl;
//     std::cout<<"Time pop: "<<time_pop<<std::endl;
//     std::cout<<"Time check adj: "<<time_check_adj<<std::endl;
//     std::cout<<"Time eval: "<<time_eval<<std::endl;
//     std::cout<<"Time set initial: "<<time_set_inital<<std::endl;
//     std::cout<<"Time calc: "<<time_calc<<std::endl;
//     std::cout<<"eval count: "<<eval_count<<std::endl;
//     std::cout<<"eval count depth: "<<eval_count_depth[0]<<" "<<eval_count_depth[1]<<" "<<eval_count_depth[2]<<std::endl;
//     std::cout<<"path: "<<std::endl;
//     for(int i=0; i<path.size(); i++){
//         std::cout<<path[i]<<" ";
//     }

//     return 0;
// }

