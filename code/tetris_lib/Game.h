/*

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include <iostream>
#include <vector>

#include "Piece.h"
#include "Collision.h"
#include "AIModel.h"
#include "constants.h"
#include "AIHeuristics.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

/*
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
 
std::vector<std::vector<std::vector<int>>> shape_2 = {
    // I
    std::vector<std::vector<int>>{
        {1, 1, 1, 1}
    },
    // T
    std::vector<std::vector<int>>{
        {0, 1, 0},
        {1, 1, 1}
    },
    // L
    std::vector<std::vector<int>>{
        {0, 0, 1},
        {1, 1, 1}
    },
    // J
    std::vector<std::vector<int>>{
        {1, 0, 0},
        {1, 1, 1}
    },
    // O
    std::vector<std::vector<int>>{
        {1, 1},
        {1, 1}
    },
    // S
    std::vector<std::vector<int>>{
        {0, 1, 1},
        {1, 1, 0}
    },
    // Z
    std::vector<std::vector<int>>{
        {1, 1, 0},
        {0, 1, 1}
    }
};

std::vector<std::vector<int>> COLOR_PIECE_INFO = std::vector<std::vector<int>>{
                {0x00, 0xff, 0xff}, // I
                {0x80, 0x00, 0x80}, // T
                {0xff, 0xa5, 0x00}, // L
                {0x00, 0x00, 0xff}, // J
                {0xff, 0xff, 0x00}, // O
                {0x00, 0xff, 0x00}, // S
                {0xff, 0x00, 0x00},  // Z
                {0x20, 0x20, 0x20}  // Blank
            };

std::vector<std::vector<int>> COLOR_BORDER_INFO = std::vector<std::vector<int>>{
                {0xdf, 0xdf, 0xdf}, // HEADER
                {0x77, 0x77, 0x77}, // HOLD
                {0x31, 0x31, 0x31}, // PIECE_DEAD
                {0xe0, 0xe0, 0xe0}, // PIECE_ALIVE
                {0xe3, 0x83, 0x83} // GAME_BORDER
            };

std::vector<std::vector<int>> COLOR_PIECE_INSIDE_INFO = std::vector<std::vector<int>>{
                {0x00, 0xc8, 0xc8}, // I
                {0x64, 0x00, 0x64}, // T
                {0xc8, 0x82, 0x00}, // L
                {0x00, 0x00, 0xc8}, // J
                {0xc8, 0xc8, 0x00}, // O
                {0x00, 0xc8, 0x00}, // S
                {0xc8, 0x00, 0x00}, // Z
                {0x20, 0x20, 0x20} // Blank
            };

class Game{
    public:
        PieceRandomizer piece_randomizer;
        Piece piece;
        Piece ghost_piece;
        Piece hold_piece;
        std::vector<Piece> piece_queue;
        bool alive;
        bool limbo;
        std::vector<std::vector<int>> board_real;
        int pending_lines;
        int last_action_key;
        Game* enemy_game;
        bool hold_used;

        SDL_Rect COORDS_BORDER;

        std::vector<std::vector<SDL_Rect>> COORDS_DISPLAY;
        std::vector<std::vector<SDL_Rect>> COORDS_DISPLAY_INSIDE;
        SDL_Rect COORDS_DISPLAY_BORDER;

        std::vector<SDL_Rect> COORDS_HOLD;
        std::vector<SDL_Rect> COORDS_HOLD_PIECE;

        std::vector<SDL_Rect> COORDS_QUEUE;
        std::vector<std::vector<std::vector<std::vector<SDL_Rect>>>> COORDS_QUEUE_PIECE_LIST;

        int PIECE_INSIDE_SIZE;
        int X;
        int Y;
        int X2;
        int Y2;
        bool pendingEvaluation;
        std::vector<std::vector<int>> COLOR_PIECE;
        std::vector<std::vector<int>> COLOR_BORDER;
        std::vector<std::vector<int>> COLOR_PIECE_INSIDE;

        int cnt;

        int current_combo;
        int current_b2b;

        Game(PieceRandomizer pieceRandomizer){
            this->piece_randomizer = pieceRandomizer;
            this->piece = this->piece_randomizer.nextPiece();
            this->ghost_piece = this->piece;
            this->hold_piece = Piece();
            this->piece_queue = std::vector<Piece>(5);
            for(int i = 0; i < 5; i++){
                this->piece_queue[i] = this->piece_randomizer.nextPiece();
            }

            this->alive = true;
            this->limbo = false;

            this->board_real = std::vector<std::vector<int>>(24, std::vector<int>(10, 0));

            this->pending_lines = 0;
            this->last_action_key = -1;

            this->enemy_game = nullptr;

            this->hold_used = false;

            this->COORDS_BORDER = SDL_Rect();

            this->COORDS_DISPLAY = std::vector<std::vector<SDL_Rect>>();
            this->COORDS_DISPLAY_INSIDE = std::vector<std::vector<SDL_Rect>>();
            this->COORDS_DISPLAY_BORDER = SDL_Rect();

            this->COORDS_HOLD = std::vector<SDL_Rect>();
            this->COORDS_HOLD_PIECE = std::vector<SDL_Rect>();

            this->COORDS_QUEUE = std::vector<SDL_Rect>();
            this->COORDS_QUEUE_PIECE_LIST = std::vector<std::vector<std::vector<std::vector<SDL_Rect>>>>();
            this->PIECE_INSIDE_SIZE = 6;
            this->X = -1;
            this->Y = -1;
            this->X2 = -1;
            this->Y2 = -1;

            this->pendingEvaluation = false;

            this->COLOR_PIECE = COLOR_PIECE_INFO;
            this->COLOR_BORDER = COLOR_BORDER_INFO;
            this->COLOR_PIECE_INSIDE = COLOR_PIECE_INSIDE_INFO;

            // std::cout<<COLOR_BORDER.size()<<std::endl;
            // for(int i = 0; i < COLOR_BORDER.size(); i++){
            //      std::cout << COLOR_BORDER[i][0] << " " << COLOR_BORDER[i][1] << " " << COLOR_BORDER[i][2] << std::endl;
            // }

            this->cnt = 0; 

            this->current_combo = 0;
            this->current_b2b = 0;

        };

        void reset(){
            this->piece_randomizer.index = 0;
            this->piece = this->piece_randomizer.nextPiece();
            this->ghost_piece = this->piece;
            this->hold_piece = Piece();
            this->piece_queue = std::vector<Piece>(5);
            for(int i = 0; i < 5; i++){
                this->piece_queue[i] = this->piece_randomizer.nextPiece();
            }

            this->alive = true;
            this->limbo = false;

            this->board_real = std::vector<std::vector<int>>(24, std::vector<int>(10, 0));

            this->pending_lines = 0;
            this->last_action_key = -1;

            this->enemy_game = nullptr;

            this->hold_used = false;
        }


        int lines_to_send_formula(int full_lines, bool is_t_spin, bool all_clear){
            int lines = 0;

            if (full_lines == 0){
                return 0;
            }

            switch(full_lines){
                case 1:
                    lines = is_t_spin ? 2 : 0;
                    break;
                case 2:
                    lines = is_t_spin ? 4 : 1;
                    break;
                case 3:
                    lines = is_t_spin ? 6 : 2;
                    break;
                case 4:
                    lines = is_t_spin ? 10 : 4;
                    break;
            }

            if (all_clear){
                lines += 10;
            }

            switch(this->current_combo){
                case 0:
                    break;
                case 1:
                    lines += 1;
                    break;
                case 2:
                    lines += 1;
                    break;
                case 3:
                    lines += 2;
                    break;
                case 4:
                    lines += 2;
                    break;
                case 5:
                    lines += 3;
                    break;
                case 6:
                    lines += 3;
                    break;
                case 7:
                    lines += 4;
                    break;
                case 8:
                    lines += 4;
                    break;
                case 9:
                    lines += 5;
                    break;
                default:
                    lines += 5;
                    break;
            }

            lines += this->current_b2b;

            return lines;
        }

        int check_full_lines(int last_action_key){
            std::vector<int> full_lines = std::vector<int>();
            bool all_clear = true;
            bool is_t_spin = isTSpin(this->piece, this->board_real, last_action_key);

            for(int i = 0; i < 24; i++){
                bool full = true;
                bool empty = true;

                for(int j = 0; j < 10; j++){
                    if (this->board_real[i][j] == 0){
                        full = false;
                    }else{
                        empty = false;
                    }
                }

                if (full){
                    full_lines.push_back(i);
                }

                all_clear = all_clear && (empty || full);
            }

            for(int i = 0; i < full_lines.size(); i++){
                this->board_real.erase(this->board_real.begin() + full_lines[i]);
                this->board_real.insert(this->board_real.begin(), std::vector<int>(10, 0));
            }

            // break combo & b2b
            if(full_lines.size() == 0){
                this->current_combo = 0;
                this->current_b2b = 0;
                return 0;
            }

            // inc combo
            this->current_combo++;

            // check b2b
            if (is_t_spin || full_lines.size() >= 4){
                this->current_b2b++;
            }else{
                this->current_b2b = 0;
            }

            // calc garbage lines
            int lines_to_send = this->lines_to_send_formula((int)full_lines.size(), is_t_spin, all_clear);

            int lines_neutralized = std::min(this->pending_lines, lines_to_send);

            this->pending_lines -= lines_neutralized;

            if(is_t_spin){
                std::cout<<"T-SPIN"<<std::endl;
            }
            // if(full_lines.size() >= 2){
                // std::cout<<"line sent"<<std::endl;
                // std::cout<<this->pending_lines<<" "<<lines_neutralized<<" "<<lines_to_send<<" "<<(int)full_lines.size()<<std::endl;
            // }

            if(lines_to_send > 0){
                request_send_lines(lines_to_send);
            }  

            return lines_to_send;

        }

        bool append_pending_lines(){
            if (this->pending_lines == 0){
                return true;
            }

            // std::cout<<"appending pending lines"<<std::endl;

            int hole = rand() % 10;
            for(int i = 0; i < this->pending_lines; i++){
                this->board_real.erase(this->board_real.begin());
                this->board_real.push_back(std::vector<int>(10, 8));
                this->board_real[23][hole] = 0;
            }
            this->pending_lines = 0;
            return true;
        }

        bool find_next_piece(){
            this->cnt++;
            if (this->piece_queue.size() == 0){
                return false;
            }

            this->piece = this->piece_queue[0];
            this->piece.setPositionRotation(5 - shape_2[this->piece.shapeNum][0].size() / 2, 0, 0);
            this->piece_queue.erase(this->piece_queue.begin());
            this->piece_queue.push_back(this->piece_randomizer.nextPiece());

            // std::cout<<this->cnt<<std::endl;
            // std::cout<<this->piece.shapeNum<<std::endl;
            // std::cout<<this->piece.x<<" "<<this->piece.y<<std::endl;
            return true;
        }

        bool check_death(){
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 10; j++){
                    if (this->board_real[i][j] != 0){
                        return true;
                    }
                }
            }
            return false;
        }

        bool lock_piece(){
            place_piece(this->piece, this->board_real);
            check_full_lines(this->last_action_key);
            append_pending_lines();

            this->hold_used = false;

            if (this->check_death()){
                this->on_self_death();
                return true;
            }

            if (!this->find_next_piece()){
                assert(false);
                return false;
            }

            return true;
        }


        void hold_current_piece(){
            if (this->hold_used){
                return;
            }

            this->hold_used = true;

            if (this->hold_piece.shapeNum == -1){
                this->hold_piece = this->piece;
                this->find_next_piece();
                return;
            }

            Piece temp = this->piece;
            this->piece = this->hold_piece;
            this->hold_piece = temp;
            this->piece.setPositionRotation(5 - shape_2[this->piece.shapeNum][0].size() / 2, 0, 0);
        }

        void perform_action(int action_key){
            if(action_key == HOLD){
                hold_current_piece();
                return;
            }
            bool result = actionFromKey(this->board_real, this->piece, action_key);
            this->last_action_key = action_key;
            
            if (!result) return;

            if (!this->lock_piece()){
                this->alive = false;
                return;
            }
        }

        // ------------------------ DEATH AND LINE CLEARING ------------------------

        void set_enemy_game(Game* enemy_game){
            this->enemy_game = enemy_game;
            this->limbo = false;
        }

        void on_self_death(){
            this->alive = false;
            this->limbo = false;
            if (this->enemy_game){
                this->enemy_game->on_enemy_death();
            }
        }

        void on_enemy_death(){
            this->limbo = true;
        }

        void on_enemy_line_sent(int lines){
            // std::cout<<"lines received"<<std::endl;
            this->pending_lines += lines;

        }

        void request_send_lines(int lines){
            // if enemy game is alive
            // std::cout<<"sending lines"<<std::endl;
            if ( this->enemy_game->alive ){
                // std::cout<<"sending lines, enemy alive"<<std::endl;
                this->enemy_game->on_enemy_line_sent(lines);
            }
        }


        // ------------------------ DRAWING ------------------------

        bool load_coords_simple(int tile_size, int offset_x = 0, int offset_y = 0){
            return this->load_coords(offset_x, offset_y, tile_size, tile_size*(10+5+5)+tile_size, tile_size*26);
        }

        bool load_coords_next_to(int adj_x, int adj_y, Game game, int tile_size, int margin){
            return this->load_coords(game.X + (game.X2 - game.X)*adj_x + margin*adj_x, 
                                     game.Y + (game.Y2 - game.Y)*adj_y + margin*adj_y, 
                                     tile_size, 
                                     game.X2 - game.X, 
                                     game.Y2 - game.Y);

        }

        bool load_coords(int offset_x, int offset_y, int tile_size, int width, int height){
            this->X = offset_x;
            this->Y = offset_y;
            this->X2 = offset_x + width;
            this->Y2 = offset_y + height;
            this->PIECE_INSIDE_SIZE = (tile_size) * (0.58);

            int oy = this->Y + 4 * tile_size;
            
            this->COORDS_BORDER = SDL_Rect{this->X, this->Y, width, height};

            int diff = this->X2 - this->X - 5 * tile_size - 10 * tile_size - 5 * tile_size;
            assert(diff >= 0);
            int margin = (int)diff / 4;
            
            this->COORDS_HOLD = std::vector<SDL_Rect>{SDL_Rect{this->X + margin, oy, 5 * tile_size, (3+1) * tile_size},
                                                      SDL_Rect{this->X + margin, oy, 5 * tile_size, tile_size}};
            this->COORDS_HOLD_PIECE = std::vector<SDL_Rect>();
            for(int i = 0; i < 7; i++){
                std::vector<std::vector<int>> shape = shape_2[i];
                int r = shape.size();
                int c = shape[0].size();

                int o_x = (5-c)*tile_size/2 + this->X + margin;
                int o_y = (3-r)*tile_size/2 + oy + tile_size;

                for(int y = 0; y < r; y++){
                    for(int x = 0; x < c; x++){
                        this->COORDS_HOLD_PIECE.push_back(SDL_Rect{o_x + x * tile_size, o_y + y * tile_size, tile_size, tile_size});
                    }
                }
            }

            int disX = this->X + margin + 5 * tile_size + margin;

            this->COORDS_DISPLAY = std::vector<std::vector<SDL_Rect>>();
            this->COORDS_DISPLAY_INSIDE = std::vector<std::vector<SDL_Rect>>();
            this->COORDS_DISPLAY_BORDER = SDL_Rect{disX, oy, 10 * tile_size, 20 * tile_size};
            for(int i = 0; i < 24; i++){
                this->COORDS_DISPLAY.push_back(std::vector<SDL_Rect>());
                this->COORDS_DISPLAY_INSIDE.push_back(std::vector<SDL_Rect>());
                for(int j = 0; j < 10; j++){
                    this->COORDS_DISPLAY[i].push_back(SDL_Rect{disX + j * tile_size, this->Y + i * tile_size, tile_size, tile_size});
                    this->COORDS_DISPLAY_INSIDE[i].push_back(SDL_Rect{disX + j * tile_size + (tile_size - this->PIECE_INSIDE_SIZE) / 2, this->Y + i * tile_size + (tile_size - this->PIECE_INSIDE_SIZE) / 2, this->PIECE_INSIDE_SIZE, this->PIECE_INSIDE_SIZE});
                }
            }

            int queueX = this->X2 - 5 * tile_size - margin;
            this->COORDS_QUEUE = std::vector<SDL_Rect>{SDL_Rect{queueX, oy, 5 * tile_size, (5*3+1) * tile_size},
                                                      SDL_Rect{queueX, oy, 5 * tile_size, tile_size}};
            this->COORDS_QUEUE_PIECE_LIST = std::vector<std::vector<std::vector<std::vector<SDL_Rect>>>>();
            for(int i = 0; i < 5; i++){
                this->COORDS_QUEUE_PIECE_LIST.push_back(std::vector<std::vector<std::vector<SDL_Rect>>>());
                for(int j = 0; j < 7; j++){
                    std::vector<std::vector<int>> shape = shape_2[j];
                    int r = shape.size();
                    int c = shape[0].size();

                    int o_x = (5-c)*tile_size/2 + queueX;
                    int o_y = (3-r)*tile_size/2 + oy + tile_size + i * 3 * tile_size;

                    this->COORDS_QUEUE_PIECE_LIST[i].push_back(std::vector<std::vector<SDL_Rect>>());
                    for(int y = 0; y < r; y++){
                        this->COORDS_QUEUE_PIECE_LIST[i][j].push_back(std::vector<SDL_Rect>());
                        for(int x = 0; x < c; x++){
                            this->COORDS_QUEUE_PIECE_LIST[i][j][y].push_back(SDL_Rect{o_x + x * tile_size, o_y + y * tile_size, tile_size, tile_size});
                        }
                    }
                }
            }

            this->COLOR_BORDER = COLOR_BORDER;
            this->COLOR_PIECE = COLOR_PIECE;
            this->COLOR_PIECE_INSIDE = COLOR_PIECE_INSIDE;

            // std::cout<<COLOR_BORDER.size()<<std::endl;
            for(int i = 0; i < COLOR_BORDER.size(); i++){
                // std::cout << COLOR_BORDER[i][0] << " " << COLOR_BORDER[i][1] << " " << COLOR_BORDER[i][2] << std::endl;
            }

            return true;
        }

        void draw(SDL_Renderer *renderer){
            // std::cout<<"DRAW"<<std::endl;

            // draw hold border
            SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[1][0], this->COLOR_BORDER[1][1], this->COLOR_BORDER[1][2], 255);
            SDL_RenderDrawRect(renderer, &this->COORDS_HOLD[0]);

            // std::cout<<"DRAW 1"<<std::endl;

            SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[0][0], this->COLOR_BORDER[0][1], this->COLOR_BORDER[0][2], 255);
            SDL_RenderDrawRect(renderer, &this->COORDS_HOLD[1]);

            // std::cout<<"DRAW 2"<<std::endl;

            // draw individual hold piece
            if(this->hold_piece.shapeNum != 7){
                for(int i = 0; i < shape_2[this->hold_piece.shapeNum].size(); i++){
                    for(int j = 0; j < shape_2[this->hold_piece.shapeNum][0].size(); j++){
                        if(shape_2[this->hold_piece.shapeNum][i][j]){
                            SDL_SetRenderDrawColor(renderer, this->COLOR_PIECE[this->hold_piece.shapeNum][0], this->COLOR_PIECE[this->hold_piece.shapeNum][1], this->COLOR_PIECE[this->hold_piece.shapeNum][2], 255);
                            SDL_RenderFillRect(renderer, &this->COORDS_HOLD_PIECE[i * shape_2[this->hold_piece.shapeNum][0].size() + j]);
                            SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[3][0], this->COLOR_BORDER[3][1], this->COLOR_BORDER[3][2], 255);
                            SDL_RenderDrawRect(renderer, &this->COORDS_HOLD_PIECE[i * shape_2[this->hold_piece.shapeNum][0].size() + j]);
                        }
                        else{
                            SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[2][0], this->COLOR_BORDER[2][1], this->COLOR_BORDER[2][2], 255);
                            SDL_RenderDrawRect(renderer, &this->COORDS_HOLD_PIECE[i * shape_2[this->hold_piece.shapeNum][0].size() + j]);
                        }
                    }
                }
            }


            // draw queue border
            SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[1][0], this->COLOR_BORDER[1][1], this->COLOR_BORDER[1][2], 255);
            SDL_RenderDrawRect(renderer, &this->COORDS_QUEUE[0]);
            SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[0][0], this->COLOR_BORDER[0][1], this->COLOR_BORDER[0][2], 255);
            SDL_RenderDrawRect(renderer, &this->COORDS_QUEUE[1]);

            // draw queue piece border
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < shape_2[this->piece_queue[i].shapeNum].size(); j++){
                    for(int k = 0; k < shape_2[this->piece_queue[i].shapeNum][0].size(); k++){
                        if(shape_2[this->piece_queue[i].shapeNum][j][k]){
                            SDL_SetRenderDrawColor(renderer, this->COLOR_PIECE[this->piece_queue[i].shapeNum][0], this->COLOR_PIECE[this->piece_queue[i].shapeNum][1], this->COLOR_PIECE[this->piece_queue[i].shapeNum][2], 255);
                            SDL_RenderFillRect(renderer, &this->COORDS_QUEUE_PIECE_LIST[i][this->piece_queue[i].shapeNum][j][k]);
                            SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[3][0], this->COLOR_BORDER[3][1], this->COLOR_BORDER[3][2], 255);
                            SDL_RenderDrawRect(renderer, &this->COORDS_QUEUE_PIECE_LIST[i][this->piece_queue[i].shapeNum][j][k]);
                        }
                        else{
                            SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[2][0], this->COLOR_BORDER[2][1], this->COLOR_BORDER[2][2], 255);
                            SDL_RenderDrawRect(renderer, &this->COORDS_QUEUE_PIECE_LIST[i][this->piece_queue[i].shapeNum][j][k]);
                        }
                    }
                }
            }

            // draw display
            for(int i = 0; i < 10; i++){
                for(int j = 4; j < 24; j++){
                    if(this->board_real[j][i] > 0){
                        SDL_SetRenderDrawColor(renderer, this->COLOR_PIECE[this->board_real[j][i]-1][0], this->COLOR_PIECE[this->board_real[j][i]-1][1], this->COLOR_PIECE[this->board_real[j][i]-1][2], 255);
                        SDL_RenderFillRect(renderer, &this->COORDS_DISPLAY[j][i]);
                        SDL_SetRenderDrawColor(renderer, this->COLOR_PIECE_INSIDE[this->board_real[j][i]-1][0], this->COLOR_PIECE_INSIDE[this->board_real[j][i]-1][1], this->COLOR_PIECE_INSIDE[this->board_real[j][i]-1][2], 255);
                        SDL_RenderFillRect(renderer, &this->COORDS_DISPLAY_INSIDE[j][i]);
                    }
                    else{
                        SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[2][0], this->COLOR_BORDER[2][1], this->COLOR_BORDER[2][2], 255);
                        SDL_RenderDrawRect(renderer, &this->COORDS_DISPLAY[j][i]);
                    }
                }
            }

            // draw piece
            SDL_SetRenderDrawColor(renderer, this->COLOR_PIECE[this->piece.shapeNum][0], this->COLOR_PIECE[this->piece.shapeNum][1], this->COLOR_PIECE[this->piece.shapeNum][2], 255);
            SDL_RenderFillRect(renderer, &this->COORDS_DISPLAY[this->piece.shape_alt[0] + this->piece.y][this->piece.shape_alt[1] + this->piece.x]);
            SDL_RenderFillRect(renderer, &this->COORDS_DISPLAY[this->piece.shape_alt[2] + this->piece.y][this->piece.shape_alt[3] + this->piece.x]);
            SDL_RenderFillRect(renderer, &this->COORDS_DISPLAY[this->piece.shape_alt[4] + this->piece.y][this->piece.shape_alt[5] + this->piece.x]);
            SDL_RenderFillRect(renderer, &this->COORDS_DISPLAY[this->piece.shape_alt[6] + this->piece.y][this->piece.shape_alt[7] + this->piece.x]);

            SDL_SetRenderDrawColor(renderer, this->COLOR_PIECE_INSIDE[this->piece.shapeNum][0], this->COLOR_PIECE_INSIDE[this->piece.shapeNum][1], this->COLOR_PIECE_INSIDE[this->piece.shapeNum][2], 255);
            SDL_RenderFillRect(renderer, &this->COORDS_DISPLAY_INSIDE[this->piece.shape_alt[0] + this->piece.y][this->piece.shape_alt[1] + this->piece.x]);
            SDL_RenderFillRect(renderer, &this->COORDS_DISPLAY_INSIDE[this->piece.shape_alt[2] + this->piece.y][this->piece.shape_alt[3] + this->piece.x]);
            SDL_RenderFillRect(renderer, &this->COORDS_DISPLAY_INSIDE[this->piece.shape_alt[4] + this->piece.y][this->piece.shape_alt[5] + this->piece.x]);
            SDL_RenderFillRect(renderer, &this->COORDS_DISPLAY_INSIDE[this->piece.shape_alt[6] + this->piece.y][this->piece.shape_alt[7] + this->piece.x]);
            
            // draw border
            SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[4][0], this->COLOR_BORDER[4][1], this->COLOR_BORDER[4][2], 255);
            SDL_RenderDrawRect(renderer, &this->COORDS_BORDER);

            SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[0][0], this->COLOR_BORDER[0][1], this->COLOR_BORDER[0][2], 255);
            SDL_RenderDrawRect(renderer, &this->COORDS_DISPLAY_BORDER);


        }


};



class AIGame: public Game{
    public:
        std::vector<int> genetic_sequence;
        int move_cd;
        int move_cd_current;
        std::vector<int> action_queue;

        AIGame(): Game(PieceRandomizer(100)){
            this->genetic_sequence = get_default();
            this->move_cd = 0;
            this->move_cd_current = 0;
            this->action_queue = std::vector<int>();
        }

        AIGame(PieceRandomizer pieceRandomizer): Game(pieceRandomizer){
            this->genetic_sequence = get_default();
            this->move_cd = 50;
            this->move_cd_current = 0;
            this->action_queue = std::vector<int>();
        }

        void alter_genetic_sequence(std::vector<int>& spec_a, std::vector<int>& spec_b){
            this->genetic_sequence = crossover(spec_a, spec_b);
        }

        void set_genetic_sequence(std::vector<int> genetic_sequence){
            this->genetic_sequence = genetic_sequence;
        }

        std::vector<int> filter_action_queue(std::vector<int> queue){
            std::vector<int> real_queue = std::vector<int>();

            for(int i = 0; i < queue.size(); i++){
                if (queue[i] == -1){
                    continue;
                }
                real_queue.push_back(queue[i]);
            }
            bool hard_drop = false;
            for(int i = real_queue.size() - 1; i >= 0; i--){
                if (real_queue[i] == SOFT_DROP){
                    real_queue.erase(real_queue.begin() + i);
                    hard_drop = true;
                    continue;
                }
                break;
            }

            if (hard_drop){
                real_queue.push_back(HARD_DROP);
            }
            else{
                real_queue.push_back(SOFT_DROP_FORCE);
            }

            return real_queue;
        }

        bool updateDeltaTime(int dt, SDL_Renderer *renderer){
            if (this->pendingEvaluation){
                return true;
            }

            // if (this->cnt == 0){
            //     std::cout<<"cnt"<<std::endl;
            //     std::cout<<this->piece.shapeNum<<" "<<this->piece.x<<" "<<this->piece.y<<std::endl;
            //     std::cout<<this->board_real.size()<<std::endl;
            //     for(int i = 0; i < this->board_real.size(); i++){
            //         for(int j = 0; j < this->board_real[0].size(); j++){
            //             std::cout<<this->board_real[i][j]<<" ";
            //         }
            //         std::cout<<std::endl;
            //     }
    
            //     this->cnt += 1;
            // }

            this->draw(renderer);

            if (this->limbo){
                SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0xc5, 255);
                SDL_RenderFillRect(renderer, &this->COORDS_BORDER);
                return true;
            }
            if (!this->alive || this->limbo){
                return true;
            }

            this->move_cd_current += dt;

            if (this->move_cd_current >= this->move_cd){
                this->move_cd_current = 0;

                if (this->action_queue.size() > 0){
                    this->perform_action(this->action_queue[0]);
                    this->action_queue.erase(this->action_queue.begin());
                    return true;
                }

                // std::cout<<"actionqueue"<<std::endl;
                // std::cout<<this->action_queue.size()<<std::endl;
                // for(int i = 0; i < this->action_queue.size(); i++){
                //     std::cout<<this->action_queue[i]<<std::endl;
                // }

                this->pendingEvaluation = true;
                // std::cout<<piece.shapeNum<<" "<<piece.x<<" "<<piece.y<<std::endl;
                std::vector<int> desired_action = getBestAction(this->board_real, this->piece, this->genetic_sequence, this->current_combo, this->current_b2b);
                std::vector<int> hold_action = std::vector<int>();

                int score = desired_action.back();
                desired_action.pop_back();
                desired_action = filter_action_queue(desired_action);

                int hold_score = 0;
                if(this->hold_piece.shapeNum != -1){
                    hold_piece.setPositionRotation(5 - shape_2[this->hold_piece.shapeNum][0].size() / 2, 0, 0);
                    hold_action = getBestAction(this->board_real, this->hold_piece, this->genetic_sequence, this->current_combo, this->current_b2b);

                    hold_score = hold_action.back();
                    hold_action.pop_back();
                    hold_action = filter_action_queue(hold_action);
                }

                if(hold_score > score){
                    this->action_queue = hold_action;
                    this->hold_current_piece();
                }
                else{
                    this->action_queue = desired_action;
                }

                // std::cout<<"actionqueue"<<std::endl;
                // std::cout<<this->piece.shapeNum<<" "<<this->piece.x<<" "<<this->piece.y<<std::endl;
                // std::cout<<this->action_queue.size()<<std::endl;
                // for(int i = 0; i < this->action_queue.size(); i++){
                //     std::cout<<this->action_queue[i];
                // }
                // std::cout<<"";
                this->pendingEvaluation = false;
            }

            return true;
        }

};

 
int main(int argc, char *argv[])
{
 
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window *window = SDL_CreateWindow("Tetris",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, 
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    bool isRunning = true;
    SDL_Event event;

    PieceRandomizer pieceRandomizer = PieceRandomizer(100);
    
    int count = 18;
    int r = 3;
    int c = 6;

    std::vector<AIGame> game_list(count);
    std::vector<int> game_assoc = std::vector<int>(count);

    for(int i = 0; i < count; i++){
        game_list[i] = AIGame(pieceRandomizer);
        std::vector<int> built = get_prebuilt();
        std::vector<int> built2 = get_prebuilt();
        std::vector<int> cross = crossover(built, built2);
        game_list[i].set_genetic_sequence(cross);
    }

    game_list[0].load_coords_simple(10, 10, 10);
    for(int i = 0; i < count; i++){
        game_list[i].load_coords_next_to(i % c, i / c, game_list[0], 10, 10);
    }

    for(int i = 0; i < count; i += 2){
        game_list[i].set_enemy_game(&game_list[i+1]);
        game_list[i+1].set_enemy_game(&game_list[i]);
        game_assoc[i] = i+1;
        game_assoc[i+1] = i;
    }

    int death_count = 0;
    std::vector<bool> death_status = std::vector<bool>(count);
    std::vector<int> mother_genetic_sequence = std::vector<int>();
    std::vector<int> father_genetic_sequence = std::vector<int>();


    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // std::cout<<"323"<<std::endl;
        for(int i = 0; i < count; i++){
            game_list[i].updateDeltaTime(1000/60, renderer);
        }

        for(int i = 0; i < count; i++){
            if(!game_list[i].alive && !death_status[i]){
                death_status[i] = true;
                death_count += 1;
                game_assoc[game_assoc[i]] = -1;
                game_assoc[i] = -1;

                if(death_count == count-1){
                    father_genetic_sequence = game_list[i].genetic_sequence;
                    death_count = -1;

                    for(int j = 0; j < count; j++){
                        if(game_list[j].alive){
                            mother_genetic_sequence = game_list[j].genetic_sequence;

                            std::cout<<&mother_genetic_sequence<<std::endl;
                            std::cout<<&father_genetic_sequence<<std::endl;

                            for(int k = 0; k < mother_genetic_sequence.size(); k++){
                                std::cout<<mother_genetic_sequence[k]<<" ";
                            }

                            std::cout<<std::endl;
                            for(int k = 0; k < father_genetic_sequence.size(); k++){
                                std::cout<<father_genetic_sequence[k]<<" ";
                            }

                            std::cout<<std::endl;
                            std::cout<<i<<" "<<j<<std::endl;

                            break;
                        }
                    }
                }
            }
        }


        if(death_count == -1){
            for(int i = 0; i < count; i++){
                game_list[i].reset();
                game_list[i].alter_genetic_sequence(mother_genetic_sequence, father_genetic_sequence);
            }

            game_list[0].set_genetic_sequence(mother_genetic_sequence);
            game_list[count-1].set_genetic_sequence(father_genetic_sequence);

            death_count = 0;
            death_status = std::vector<bool>(count);
            mother_genetic_sequence = std::vector<int>();
            father_genetic_sequence = std::vector<int>();

            for(int i = 0; i < count; i+=2){
                game_list[i].set_enemy_game(&game_list[i+1]);
                game_list[i+1].set_enemy_game(&game_list[i]);
                game_assoc[i] = i+1;
                game_assoc[i+1] = i;
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(1000);

            continue;
        }

        for(int i = 0; i < count; i++){
            if(game_list[i].limbo){
                for(int j = 0; j < count; j++){
                    if( i != j && game_list[j].limbo){
                        game_list[i].set_enemy_game(&game_list[j]);
                        game_list[j].set_enemy_game(&game_list[i]);
                        game_assoc[i] = j;
                        game_assoc[j] = i;
                        break;
                    }
                }
            }
        }


        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 120);
    }

    return 0;
}

 */

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <vector>

#include "Piece.h"
#include "constants.h"

class Game{
    public:
        Piece piece;
        Piece ghost_piece;
        Piece hold_piece;
        std::vector<Piece> piece_queue;
        PieceRandomizer piece_randomizer;
        int board_real[24][10];
        // int** board_real;
        bool alive;
        bool limbo;
        int pending_lines;
        int last_action_key;
        Game* enemy_game;
        bool hold_used;
        bool pendingEvaluation;
        int current_combo;
        int current_b2b;

        SDL_Rect COORDS_BORDER;

        std::vector<std::vector<int>> COLOR_PIECE;
        std::vector<std::vector<int>> COLOR_BORDER;
        std::vector<std::vector<int>> COLOR_PIECE_INSIDE;

        std::vector<std::vector<SDL_Rect>> COORDS_DISPLAY;
        std::vector<std::vector<SDL_Rect>> COORDS_DISPLAY_INSIDE;
        SDL_Rect COORDS_DISPLAY_BORDER;

        std::vector<SDL_Rect> COORDS_HOLD;
        std::vector<std::vector<std::vector<SDL_Rect>>> COORDS_HOLD_PIECE;

        std::vector<SDL_Rect> COORDS_QUEUE;
        std::vector<std::vector<std::vector<std::vector<SDL_Rect>>>> COORDS_QUEUE_PIECE_LIST;
        int PIECE_INSIDE_SIZE;
        int X;
        int Y;
        int X2;
        int Y2;

        int cnt;

        int last_clear;
        bool was_t_spin;
        int lifetime_lines_sent;

        std::vector<std::string> clear_text;
        // std::vector<SDL_Texture*> clear_text_textures;
        // std::vector<SDL_Texture*> combo_text_textures;
        // std::vector<SDL_Texture*> b2b_text_textures;
        // SDL_Texture* lifetime_texture_main;
        // std::vector<SDL_Texture*> lifetime_texture_digits;

        std::vector<SDL_Texture*> clear_text_textures;
        SDL_Texture* combo_text_texture;
        SDL_Texture* b2b_text_texture;
        SDL_Texture* lifetime_texture;

        std::vector<SDL_Texture*> digit_tex;

        std::vector<SDL_Rect> clear_text_rects;
        std::vector<SDL_Rect> combo_text_rects;
        std::vector<SDL_Rect> b2b_text_rects;
        std::vector<SDL_Rect> lifetime_rects;

        SDL_Color impending_lines_color = {0xe2, 0x1b, 0x1b, 0xff};
        int impending_lines_indicator_lim = 16;
        int impending_lines_indicator_width = 4;
        SDL_Rect impending_lines_indicator_rect;

        int dig_w[10];

        // SDL_Rect clear_text_rect;
        // SDL_Rect combo_text_rect;
        // SDL_Rect b2b_text_rect;
        // SDL_Rect lifetime_rect_main;
        // std::vector<SDL_Rect> lifetime_rect_digits;

        // TTF_Font* font;

        Game(PieceRandomizer pieceRandomizer);

        void reset(int prandseed = -1);

        void setPieceRandomizer(PieceRandomizer pieceRandomizer);

        int lines_to_send_formula(int full_lines, bool is_t_spin, bool all_clear, int combo, int b2b);
        int check_full_lines(int last_action_key);
        bool append_pending_lines();
        bool find_next_piece();
        bool check_death();
        bool lock_piece();
        void hold_current_piece();
        void perform_action(int action_key);
        void set_enemy_game(Game* enemy_game);
        void on_self_death();
        void on_enemy_death();
        void on_enemy_line_sent(int lines);
        void request_send_lines(int lines);
        void draw(SDL_Renderer *renderer);
        bool load_coords(SDL_Renderer* renderer, int offset_x, int offset_y, int tile_size, int width, int height);
        bool load_coords_simple(SDL_Renderer* renderer, int tile_size, int offset_x, int offset_y);
        bool load_coords_next_to(SDL_Renderer* renderer, int adj_x, int adj_y, Game game, int tile_size, int margin);
};

std::pair<int, int> dim_from_tile_size(int tile_size);

class AIGame: public Game{
    public:
        std::vector<int> genetic_sequence;
        int pps_cd;
        int pps_cd_current;
        std::vector<int> action_queue;

        int depth;

        AIGame();
        AIGame(PieceRandomizer pieceRandomizer);

        void reset(int prandseed = -1);

        void set_depth(int depth);
        void set_pps(int pps);
        void alter_genetic_sequence(std::vector<int>& spec_a, std::vector<int>& spec_b);
        void set_genetic_sequence(std::vector<int> genetic_sequence);
        std::vector<int> filter_action_queue(std::vector<int> queue);
        bool updateDeltaTime(int dt, SDL_Renderer *renderer, bool diagnostic=false, std::vector<double>* diagnostic_data=nullptr);
};

class PlayerGame: public Game{
    public:
        int move_cd_current = 0;
        int move_cd = 5;
        int move_cd_initial = 70;
        bool move_cd_initial_flag = true;

        int soft_drop_cd_current = 0;
        int soft_drop_cd = 5;
        bool soft_drop_cd_initial_flag = true;

        int auto_fall_cd_current = 0;
        int auto_fall_cd = 1000;
        int auto_fall_grace = 1700;
        int auto_fall_grace_initial = 1700;
        int auto_fall_grace_falloff_factor = 0.95;

        std::vector<bool> key_status = std::vector<bool>(20, false);

        PlayerGame(): Game(PieceRandomizer(100)){
        }

        PlayerGame(PieceRandomizer pieceRandomizer): Game(pieceRandomizer){
        }

        void reset(int prandseed = -1){
            Game::reset(prandseed);
            this->move_cd_current = 0;
            this->soft_drop_cd_current = 0;
            this->auto_fall_cd_current = 0;
        }

        void set_cds(int initial_move_cd, int move_cd, int soft_drop_cd){
            this->move_cd_initial = initial_move_cd;
            this->move_cd = move_cd;
            this->soft_drop_cd = soft_drop_cd;
        }

        void on_key_down(SDL_Keycode& key){
            switch(key){
                case SDLK_LEFT:
                    key_status[MOVE_LEFT] = true;
                    key_status[MOVE_RIGHT] = false;
                    break;
                case SDLK_RIGHT:
                    key_status[MOVE_LEFT] = false;
                    key_status[MOVE_RIGHT] = true;
                    break;
                case SDLK_DOWN:
                    key_status[SOFT_DROP] = true;
                    break;
                case SDLK_UP:
                    key_status[ROTATE_CLOCKWISE] = true;
                    break;
                case SDLK_z:
                    key_status[ROTATE_COUNTER] = true;
                    break;
                case SDLK_c:
                    key_status[HOLD] = true;
                    break;
                case SDLK_SPACE:
                    key_status[HARD_DROP] = true;
                    break;
            }
        }
        void on_key_up(SDL_Keycode& key){
            switch(key){
                case SDLK_LEFT:
                    key_status[MOVE_LEFT] = false;
                    move_cd_initial_flag = true;
                    break;
                case SDLK_RIGHT:
                    key_status[MOVE_RIGHT] = false;
                    move_cd_initial_flag = true;
                    break;
                case SDLK_DOWN:
                    key_status[SOFT_DROP] = false;
                    soft_drop_cd_initial_flag = true;
                    break;
                case SDLK_UP:
                    key_status[ROTATE_CLOCKWISE] = false;
                    break;
                case SDLK_z:
                    key_status[ROTATE_COUNTER] = false;
                    break;
                case SDLK_c:
                    key_status[HOLD] = false;
                    break;
                case SDLK_SPACE:
                    key_status[HARD_DROP] = false;
                    break;
            }
        }

        void updateDeltaTime(int dt, SDL_Renderer *renderer);

        SDL_Color ghost_color = {0xb2, 0xb2, 0xb2, 0xff};

};


#endif // GAME_H
