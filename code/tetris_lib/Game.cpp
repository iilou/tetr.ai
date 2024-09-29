
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <vector>
#include <string>

#include "Piece.h"
#include "Collision.h"
#include "constants.h"
#include "Game.h"
#include "../model/AIModel.h"
#include "../model/AIHeuristics.h"

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
 */
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

Game::Game(PieceRandomizer pieceRandomizer){
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

    // this->board_real = std::vector<std::vector<int>>(24, std::vector<int>(10, 0));
    for(int i = 0; i < 24; i++){
        for(int j = 0; j < 10; j++){
            this->board_real[i][j] = 0;
        }
    }

    this->pending_lines = 0;
    this->last_action_key = -1;

    this->enemy_game = nullptr;

    this->hold_used = false;

    this->COORDS_BORDER = SDL_Rect();

    this->COORDS_DISPLAY = std::vector<std::vector<SDL_Rect>>();
    this->COORDS_DISPLAY_INSIDE = std::vector<std::vector<SDL_Rect>>();
    this->COORDS_DISPLAY_BORDER = SDL_Rect();

    this->COORDS_HOLD = std::vector<SDL_Rect>();
    this->COORDS_HOLD_PIECE = std::vector<std::vector<std::vector<SDL_Rect>>>();

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

    this->cnt = 0; 

    this->last_clear = 0;
    this->was_t_spin = false;
    this->lifetime_lines_sent = 0;

    this->current_combo = 0;
    this->current_b2b = 0;

    this->clear_text = {"SINGLE", "DOUBLE", "TRIPLE", "QUAD", "T-SPIN SINGLE", "T-SPIN DOUBLE", "T-SPIN TRIPLE"};
    
    this->clear_text_textures = std::vector<SDL_Texture*>();
    this->combo_text_texture = nullptr;
    this->b2b_text_texture = nullptr;
    this->lifetime_texture = nullptr;

    this->digit_tex = std::vector<SDL_Texture*>(10, nullptr);

    this->clear_text_rects = std::vector<SDL_Rect>();
    this->combo_text_rects = std::vector<SDL_Rect>();
    this->b2b_text_rects = std::vector<SDL_Rect>();
    this->lifetime_rects = std::vector<SDL_Rect>();

    this->enemy_game = nullptr;

};

void Game::reset(int prandseed){
    this->piece_randomizer.index = prandseed;

    this->piece = this->piece_randomizer.nextPiece();
    this->ghost_piece = this->piece;
    this->hold_piece = Piece();
    for(int i = 0; i < 5; i++){
        this->piece_queue[i] = this->piece_randomizer.nextPiece();
    }

    this->alive = true;
    this->limbo = false;

    // this->board_real = std::vector<std::vector<int>>(24, std::vector<int>(10, 0));
    for(int i = 0; i < 24; i++){
        for(int j = 0; j < 10; j++){
            this->board_real[i][j] = 0;
        }
    }


    this->pending_lines = 0;
    this->last_action_key = -1;

    this->enemy_game = nullptr;

    this->hold_used = false;

    this->last_clear = 0;
    this->was_t_spin = false;
    this->lifetime_lines_sent = 0;
    this->current_combo = 0;
    this->current_b2b = 0;
}

void AIGame::reset(int prandseed){
    Game::reset(prandseed);
    this->pendingEvaluation = false;
    this->action_queue = std::vector<int>();
}

void Game::setPieceRandomizer(PieceRandomizer pieceRandomizer){
    this->piece_randomizer = pieceRandomizer;
}

int Game::lines_to_send_formula(int full_lines, bool is_t_spin, bool all_clear, int combo, int b2b){
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

    switch(combo){
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


    lines += std::max(0, b2b - 1);
    return lines;
}

int Game::check_full_lines(int last_action_key){
    std::vector<int> full_lines = std::vector<int>();
    bool all_clear = true;
    bool is_t_spin = isTSpin(this->piece, this->board_real, this->last_action_key);

    if(is_t_spin){
        std::cout<<"T-SPIN"<<std::endl;
    }

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

        for(int j = 0; j < 10; j++){
            this->board_real[full_lines[i]][j] = 0;
        }
        for(int j = full_lines[i]; j > 0; j--){
            for(int k = 0; k < 10; k++){
                this->board_real[j][k] = this->board_real[j-1][k];
            }
        }
        for(int j = 0; j < 10; j++){
            this->board_real[0][j] = 0;
        }
    }

    // update combo
    if (this->last_clear > 0 && full_lines.size() > 0){
        this->current_combo++;
    }

    if (full_lines.size() == 0){
        this->current_combo = 0;
        this->last_clear = 0;
        return 0;
    }

    // update b2b
    if (full_lines.size() > 0 && (is_t_spin || full_lines.size() == 4) && (this->last_clear > 0 || this->was_t_spin)){
        this->current_b2b++;
    } else if (full_lines.size() > 0){
        this->current_b2b = 0;
    }

    this->was_t_spin = is_t_spin;
    this->last_clear = full_lines.size();

    // calc garbage lines
    int lines_to_send = this->lines_to_send_formula((int)full_lines.size(), is_t_spin, all_clear, this->current_combo, this->current_b2b);
    this->lifetime_lines_sent += lines_to_send;

    int lines_neutralized = std::min(this->pending_lines, lines_to_send);

    this->pending_lines -= lines_neutralized;


    if(lines_to_send > 0){
        request_send_lines(lines_to_send);
    }  

    return lines_to_send;

}

bool Game::append_pending_lines(){
    if (this->pending_lines == 0){
        return true;
    }

    int hole = rand() % 10;

    if(this->pending_lines > 23){
        for(int i = 0; i < 24; i++){
            for(int j = 0; j < 10; j++){
                this->board_real[i][j] = j == hole ? 0 : 8;
            }
        }

        this->pending_lines = 0;
        return true;
    }

    for(int i = this->pending_lines; i < 24; i++){
        for(int j = 0; j < 10; j++){
            this->board_real[i-this->pending_lines][j] = this->board_real[i][j];
        }
    }
    for(int i = 24-this->pending_lines; i < 24; i++){
        for(int j = 0; j < 10; j++){
            this->board_real[i][j] = j == hole ? 0 : 8;
        }
    }
    this->pending_lines = 0;
    return true;
}

bool Game::find_next_piece(){
    this->cnt++;
    if (this->piece_queue.size() == 0){
        return false;
    }

    this->piece = this->piece_queue[0];
    this->piece.setPositionRotation(5 - shape_2[this->piece.shapeNum][0].size() / 2, 0, 0);
    this->piece_queue.erase(this->piece_queue.begin());
    this->piece_queue.push_back(this->piece_randomizer.nextPiece());

    return true;
}

bool Game::check_death(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 10; j++){
            if (this->board_real[i][j] != 0){
                return true;
            }
        }
    }
    return false;
}

bool Game::lock_piece(){
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

void Game::hold_current_piece(){
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

void Game::perform_action(int action_key){
            if(action_key == HOLD){
                hold_current_piece();
                return;
            }
            bool result = actionFromKey(this->board_real, this->piece, action_key);
            
            if (!result){
                this->last_action_key = action_key;
                return;
            } 

            if (!this->lock_piece()){
                this->alive = false;
                return;
            }
        }

        // ------------------------ DEATH AND LINE CLEARING ------------------------

void Game::set_enemy_game(Game* enemy_game){
    this->enemy_game = enemy_game;
    this->limbo = false;
}

void Game::on_self_death(){
    this->alive = false;
    this->limbo = false;
    if (this->enemy_game != nullptr){
        this->enemy_game->on_enemy_death();
    }
}

void Game::on_enemy_death(){
    this->limbo = true;
}

void Game::on_enemy_line_sent(int lines){
    this->pending_lines += lines;
}

void Game::request_send_lines(int lines){
    if ( this->enemy_game != nullptr && this->enemy_game->alive){
        this->enemy_game->on_enemy_line_sent(lines);
    }
}


        // ------------------------ DRAWING ------------------------

bool Game::load_coords_simple(SDL_Renderer* renderer, int tile_size, int offset_x = 0, int offset_y = 0){
            return this->load_coords(renderer, offset_x, offset_y, tile_size, tile_size*(10+5+5)+tile_size, tile_size*26);
        }

std::pair<int, int> dim_from_tile_size(int tile_size){
    return std::pair<int, int>(tile_size*(10+5+5)+tile_size, tile_size*26);
}

bool Game::load_coords_next_to(SDL_Renderer* renderer, int adj_x, int adj_y, Game game, int tile_size, int margin){
            return this->load_coords(renderer,
                                    game.X + (game.X2 - game.X)*adj_x + margin*adj_x, 
                                    game.Y + (game.Y2 - game.Y)*adj_y + margin*adj_y, 
                                    tile_size, 
                                    game.X2 - game.X, 
                                    game.Y2 - game.Y);
        }

bool Game::load_coords(SDL_Renderer* renderer, int offset_x, int offset_y, int tile_size, int width, int height){
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
    this->COORDS_HOLD_PIECE = std::vector<std::vector<std::vector<SDL_Rect>>>();
    for(int i = 0; i < 7; i++){
        this->COORDS_HOLD_PIECE.push_back(std::vector<std::vector<SDL_Rect>>());
        std::vector<std::vector<int>> shape = shape_2[i];
        int r = shape.size();
        int c = shape[0].size();

        int o_x = (5-c)*tile_size/2 + this->X + margin;
        int o_y = (3-r)*tile_size/2 + oy + tile_size;

        for(int y = 0; y < r; y++){
            this->COORDS_HOLD_PIECE[i].push_back(std::vector<SDL_Rect>());
            for(int x = 0; x < c; x++){
                this->COORDS_HOLD_PIECE[i][y].push_back(SDL_Rect{o_x + x * tile_size, o_y + y * tile_size, tile_size, tile_size});
            }
        }
    }

    // ---------------------------- FONT ----------------------------
    TTF_Font* font = TTF_OpenFont("HunDIN1451.ttf", tile_size);

    int dig_max_w = 0;
    // dig_w = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // -- DIGITS --
    for(int i = 0; i < 10; i++){
        SDL_Surface* surface = TTF_RenderText_Solid(font, std::to_string(i).c_str(), SDL_Color{255, 255, 255});
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        dig_max_w = std::max(dig_max_w, surface->w * tile_size / surface->h);
        dig_w[i] = surface->w * tile_size / surface->h;
        this->digit_tex[i] = texture;
        SDL_FreeSurface(surface);
    }

    // -- CLEAR TEXT --
    int cb_off_y = oy + 5 * tile_size + margin;
    // this->clear_text_rect = SDL_Rect{this->X + margin, cb_off_y, 5 * tile_size, tile_size};
    for(int i = 0; i < this->clear_text.size(); i++){
        SDL_Surface* surface = TTF_RenderText_Solid(font, this->clear_text[i].c_str(), SDL_Color{255, 255, 255});
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        this->clear_text_textures.push_back(texture);
        this->clear_text_rects.push_back(SDL_Rect{this->X + margin, cb_off_y + i * tile_size, surface->w * tile_size / surface->h, tile_size});
        SDL_FreeSurface(surface);
    }

    // -- COMBO TEXT --
    int combo_off_y = cb_off_y + tile_size + 2;
    // this->combo_text_rect = SDL_Rect{this->X + margin, combo_off_y, 5 * tile_size, tile_size};
    std::string text = "COMBO x";
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), SDL_Color{255, 255, 255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    this->combo_text_texture = texture;
    int texW = surface->w * tile_size / surface->h;
    this->combo_text_rects.push_back(SDL_Rect{this->X + margin, combo_off_y, texW, tile_size});
    SDL_FreeSurface(surface);
    for(int i = 0; i < 2; i++){ // limit 99
        this->combo_text_rects.push_back(SDL_Rect{this->X + margin + texW + i * dig_max_w, combo_off_y, dig_max_w, tile_size});
    }

    // -- B2B TEXT --
    int b2b_off_y = combo_off_y + tile_size + 2;
    SDL_Surface* surface2 = TTF_RenderText_Solid(font, "B2B x", SDL_Color{255, 255, 255});
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    this->b2b_text_texture = texture2;
    int texW2 = surface2->w * tile_size / surface2->h;
    this->b2b_text_rects.push_back(SDL_Rect{this->X + margin, b2b_off_y, texW2, tile_size});
    SDL_FreeSurface(surface2);
    for(int i = 0; i < 3; i++){ // limit 999
        this->b2b_text_rects.push_back(SDL_Rect{this->X + margin + texW2 + i * dig_max_w, b2b_off_y, dig_max_w, tile_size});
    }



    // ---------------------------- ACTUAL BOARD ----------------------------
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

    this->impending_lines_indicator_rect = SDL_Rect{COORDS_DISPLAY_BORDER.x-impending_lines_indicator_width, 0, impending_lines_indicator_width, 0};

    // ---------------------------- QUEUE ----------------------------
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

    
    // -- LIFETIME TEXT --
    int life_off_y = oy + ((5*3+1) * tile_size) + margin;
    SDL_Surface* surface3 = TTF_RenderText_Solid(font, "PTS ", SDL_Color{255, 255, 255});
    SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
    this->lifetime_texture = texture3;
    int texW3 = surface3->w * tile_size / surface3->h;
    this->lifetime_rects.push_back(SDL_Rect{queueX, life_off_y, texW3, tile_size});
    SDL_FreeSurface(surface3);
    for(int i = 0; i < 3; i++){ // limit 999
        this->lifetime_rects.push_back(SDL_Rect{queueX + texW3 + i * dig_max_w, life_off_y, dig_max_w, tile_size});
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

void Game::draw(SDL_Renderer *renderer){

    SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[1][0], this->COLOR_BORDER[1][1], this->COLOR_BORDER[1][2], 255);
    SDL_RenderDrawRect(renderer, &this->COORDS_HOLD[0]);


    SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[0][0], this->COLOR_BORDER[0][1], this->COLOR_BORDER[0][2], 255);
    SDL_RenderDrawRect(renderer, &this->COORDS_HOLD[1]);

    // draw individual hold piece
    if(this->hold_piece.shapeNum != -1){
        for(int i = 0; i < shape_2[this->hold_piece.shapeNum].size(); i++){ // r
            for(int j = 0; j < shape_2[this->hold_piece.shapeNum][0].size(); j++){ // c
                if(shape_2[this->hold_piece.shapeNum][i][j]){
                    SDL_SetRenderDrawColor(renderer, this->COLOR_PIECE[this->hold_piece.shapeNum][0], this->COLOR_PIECE[this->hold_piece.shapeNum][1], this->COLOR_PIECE[this->hold_piece.shapeNum][2], 255);
                    SDL_RenderFillRect(renderer, &this->COORDS_HOLD_PIECE[this->hold_piece.shapeNum][i][j]);
                    SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[3][0], this->COLOR_BORDER[3][1], this->COLOR_BORDER[3][2], 255);
                    SDL_RenderDrawRect(renderer, &this->COORDS_HOLD_PIECE[this->hold_piece.shapeNum][i][j]);
                }
                else{
                    SDL_SetRenderDrawColor(renderer, this->COLOR_BORDER[2][0], this->COLOR_BORDER[2][1], this->COLOR_BORDER[2][2], 255);
                    SDL_RenderDrawRect(renderer, &this->COORDS_HOLD_PIECE[this->hold_piece.shapeNum][i][j]);
                }
            }
        }
    }

    // draw text
    if(this->last_clear > 0){
        // SDL_RenderCopy(renderer, this->clear_text_textures[this->last_clear - 1 + 4*this->was_t_spin], NULL, &this->clear_text_rect);
        SDL_RenderCopy(renderer, this->clear_text_textures[std::min(this->last_clear - 1 + 4*this->was_t_spin, 6)], NULL, &this->clear_text_rects[std::min(this->last_clear - 1 + 4*this->was_t_spin, 6)]);
    }
    if(this->current_combo > 0){
        // SDL_RenderCopy(renderer, this->combo_text_textures[std::min(this->current_combo-1, 9)], NULL, &this->combo_text_rect);
        SDL_RenderCopy(renderer, this->combo_text_texture, NULL, &this->combo_text_rects[0]);
        int num = this->current_combo;
        //this->combo_text_rect: [MSB, LSB]
        for(int i = 1; i >= 0; i--){
            this->combo_text_rects[i+1].w = dig_w[num % 10];
            SDL_RenderCopy(renderer, this->digit_tex[num % 10], NULL, &this->combo_text_rects[i+1]);
            num /= 10;
        }
    }
    if(this->current_b2b > 0){
        // SDL_RenderCopy(renderer, this->b2b_text_textures[std::max(std::min(this->current_b2b, 24), 0)], NULL, &this->b2b_text_rect);
        SDL_RenderCopy(renderer, this->b2b_text_texture, NULL, &this->b2b_text_rects[0]);
        int num = this->current_b2b;
        //this->b2b_text_rect: [MSB, 2ND, LSB]
        for(int i = 2; i >= 0; i--){
            this->b2b_text_rects[i+1].w = dig_w[num % 10];
            SDL_RenderCopy(renderer, this->digit_tex[num % 10], NULL, &this->b2b_text_rects[i+1]);
            num /= 10;
        }
    }

    SDL_RenderCopy(renderer, this->lifetime_texture, NULL, &this->lifetime_rects[0]);

    int num = this->lifetime_lines_sent;
    //this->lifetime_rect: [MSB, 2ND, LSB]
    for(int i = 2; i >= 0; i--){
        this->lifetime_rects[i+1].w = dig_w[num % 10];
        SDL_RenderCopy(renderer, this->digit_tex[num % 10], NULL, &this->lifetime_rects[i+1]);
        num /= 10;
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

    // draw impending lines
    if(this->pending_lines > 0){
        SDL_SetRenderDrawColor(renderer, impending_lines_color.r, impending_lines_color.g, impending_lines_color.b, 255);
        double percentage = (double)this->pending_lines / this->impending_lines_indicator_lim;
        this->impending_lines_indicator_rect.h = (int)(percentage * this->COORDS_DISPLAY_BORDER.h);
        this->impending_lines_indicator_rect.y = this->COORDS_DISPLAY_BORDER.y + (int)((double)(1.0 - percentage) * this->COORDS_DISPLAY_BORDER.h);
        // std::cout<<percentage<<std::endl;
        SDL_RenderFillRect(renderer, &this->impending_lines_indicator_rect);
    }
}




AIGame::AIGame(): Game(PieceRandomizer(100)){
    this->genetic_sequence = get_default();
    this->pps_cd = 100;
    this->pps_cd_current = 0;
    this->action_queue = std::vector<int>();
    this->depth = 1;
}

AIGame::AIGame(PieceRandomizer pieceRandomizer): Game(pieceRandomizer){
    this->genetic_sequence = get_default();
    this->pps_cd = 100;
    this->pps_cd_current = 0;
    this->action_queue = std::vector<int>();
    this->depth = 1;
}

void AIGame::set_depth(int depth){
    this->depth = depth;
}

void AIGame::set_pps(int pps){
    this->pps_cd = 1000 / pps;
}

void AIGame::alter_genetic_sequence(std::vector<int>& spec_a, std::vector<int>& spec_b){
    this->genetic_sequence = crossover(spec_a, spec_b);
}

void AIGame::set_genetic_sequence(std::vector<int> genetic_sequence){
    this->genetic_sequence = genetic_sequence;
}

std::vector<int> AIGame::filter_action_queue(std::vector<int> queue){
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

bool AIGame::updateDeltaTime(int dt, SDL_Renderer *renderer, bool diagnostic, std::vector<double>* diagnostic_data){
    if (this->pendingEvaluation){
        return true;
    }
    if(diagnostic){
        assert(diagnostic_data != nullptr && diagnostic_data->size() >= 4);
    }


    // draw
    if (diagnostic) diagnostic_data->at(0) = clock();
    this->draw(renderer); 
    if (diagnostic) diagnostic_data->at(1) += (double)(clock() - diagnostic_data->at(0)) / CLOCKS_PER_SEC;


    if (this->limbo){
        SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0xc5, 255);
        SDL_RenderFillRect(renderer, &this->COORDS_BORDER);
        return true;
    }
    if (!this->alive || this->limbo){
        return true;
    }

    this->pps_cd_current += dt;

    if (this->action_queue.size() > 0){
        int action = this->action_queue[0];
        for(int i = 0; i < 5; i++){
            if(this->action_queue.size() == 0 || this->action_queue[0] != action){
                break;
            }

            this->perform_action(action);   
            this->action_queue.erase(this->action_queue.begin());
        }
        return true;
    }

    if (this->pps_cd_current < this->pps_cd){ // wait for pps_cd
        return true;
    }

    this->pps_cd_current = 0;

    this->pendingEvaluation = true;
    if(diagnostic) diagnostic_data->at(0) = clock();
    std::vector<int> desired_action = getBestAction(this->board_real, this->piece, &this->hold_piece, this->piece_queue, 0, this->depth, this->genetic_sequence, this->current_combo, this->current_b2b, this->pending_lines);
    if(diagnostic) diagnostic_data->at(DIA_CHOOSE) += (double)(clock() - diagnostic_data->at(0)) / CLOCKS_PER_SEC;
    std::vector<int> hold_action = std::vector<int>();

    int score = desired_action.back();
    desired_action.pop_back();
    desired_action = filter_action_queue(desired_action);

    int hold_score = 0;
    if(this->hold_piece.shapeNum != -1){
        hold_piece.setPositionRotation(5 - shape_2[this->hold_piece.shapeNum][0].size() / 2, 0, 0);
        if(diagnostic) diagnostic_data->at(0) = clock();
        hold_action = getBestAction(this->board_real, this->hold_piece, &this->piece, this->piece_queue, 0, this->depth, this->genetic_sequence, this->current_combo, this->current_b2b, this->pending_lines);
        if(diagnostic) diagnostic_data->at(DIA_CHOOSE) += (double)(clock() - diagnostic_data->at(0)) / CLOCKS_PER_SEC;

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

    this->pendingEvaluation = false;

    return true;
}

void PlayerGame::updateDeltaTime(int dt, SDL_Renderer *renderer){

    draw(renderer);

    // render ghost piece

    // find ghost piece position
    int dy = 0;
    while(!checkCollision(this->piece, this->board_real, 0, dy)){
        dy++;
    }
    dy--;

    // draw ghost piece
    SDL_SetRenderDrawColor(renderer, ghost_color.r, ghost_color.g, ghost_color.b, ghost_color.a);
    SDL_RenderDrawRect(renderer, &this->COORDS_DISPLAY[this->piece.shape_alt[0] + this->piece.y + dy][this->piece.shape_alt[1] + this->piece.x]);
    SDL_RenderDrawRect(renderer, &this->COORDS_DISPLAY[this->piece.shape_alt[2] + this->piece.y + dy][this->piece.shape_alt[3] + this->piece.x]);
    SDL_RenderDrawRect(renderer, &this->COORDS_DISPLAY[this->piece.shape_alt[4] + this->piece.y + dy][this->piece.shape_alt[5] + this->piece.x]);
    SDL_RenderDrawRect(renderer, &this->COORDS_DISPLAY[this->piece.shape_alt[6] + this->piece.y + dy][this->piece.shape_alt[7] + this->piece.x]);


    if (this->limbo){
        SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0xc5, 255);
        SDL_RenderDrawRect(renderer, &this->COORDS_BORDER);
        return;
    }
    if (!this->alive || this->limbo){
        return;
    }

    this->move_cd_current = std::max(0, this->move_cd_current - dt);
    this->soft_drop_cd_current = std::max(0, this->soft_drop_cd_current - dt);
    this->auto_fall_cd_current = std::max(0, this->auto_fall_cd_current - dt);

    if(key_status[HOLD]){
        hold_current_piece();
        key_status[HOLD] = false;
    }

    if(this->move_cd_current == 0 && (key_status[MOVE_LEFT] || key_status[MOVE_RIGHT])){
        this->perform_action(key_status[MOVE_RIGHT]); // cool trick, will break if i change anything tho
        move_cd_current = move_cd;
        if(move_cd_initial_flag){
            move_cd_current = move_cd_initial;
            auto_fall_cd_current = std::max(auto_fall_grace, auto_fall_cd_current);
            // auto_fall_grace *= auto_fall_grace_falloff_factor;
            move_cd_initial_flag = false;
        }
    }

    if(key_status[SOFT_DROP] && this->soft_drop_cd_current == 0 && soft_drop_cd_initial_flag){
        this->soft_drop_cd_current = this->soft_drop_cd;
        soft_drop_cd_initial_flag = false;
        this->perform_action(SOFT_DROP_FORCE);
    } else if (key_status[SOFT_DROP] && this->soft_drop_cd_current == 0){
        this->soft_drop_cd_current = this->soft_drop_cd;
        this->perform_action(SOFT_DROP);
        auto_fall_cd_current = std::max(auto_fall_grace, auto_fall_cd_current);
    }

    if(key_status[ROTATE_CLOCKWISE]){
        this->perform_action(ROTATE_CLOCKWISE);
        key_status[ROTATE_CLOCKWISE] = false;
        auto_fall_cd_current = std::max(auto_fall_grace, auto_fall_cd_current);
        auto_fall_grace *= auto_fall_grace_falloff_factor;
    }

    if(key_status[ROTATE_COUNTER]){
        this->perform_action(ROTATE_COUNTER);
        key_status[ROTATE_COUNTER] = false;
        auto_fall_cd_current = std::max(auto_fall_grace, auto_fall_cd_current);
        auto_fall_grace *= auto_fall_grace_falloff_factor;
    }

    if (key_status[HARD_DROP]){
        this->perform_action(HARD_DROP);
        key_status[HARD_DROP] = false;
        return;
    }

    if(this->auto_fall_cd_current == 0){
        this->perform_action(SOFT_DROP_FORCE);
        this->auto_fall_cd_current = this->auto_fall_cd;
        this->auto_fall_grace = this->auto_fall_grace_initial;
    }
}
