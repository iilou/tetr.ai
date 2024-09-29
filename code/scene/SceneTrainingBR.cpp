#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <string>
#include <ctime>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

#include "../tetris_lib/Game.h"
#include "../tetris_lib/Piece.h"
#include "../tetris_lib/Collision.h"
#include "../tetris_lib/constants.h"
#include "../model/AIHeuristics.h"
#include "../model/AIModel.h"
#include "SceneTrainingBR.h"
#include "SCENENAV.h"


SceneTrainingBR::SceneTrainingBR(){
    R = -1;
    C = -1;
    COUNT = -1;
    death_count = 0;

    AIGAME_L = std::vector<AIGame>(10);
    gmassoc = std::vector<int>(10, 0);
    genes = get_default();
    grid = std::vector<std::vector<int>>(24, std::vector<int>(10, 0));

    death_status = std::vector<bool>(10, false);
    mother_genetic_sequence = std::vector<int>();
    father_genetic_sequence = std::vector<int>();

    t_time = 0;
    diag = std::vector<double>(10, 0.0);

    menu_tex = nullptr;
    generation_count_tex = nullptr;
    dig_tex = std::vector<SDL_Texture*>(10, nullptr);
    confirm_tex = nullptr;

    menu_rect = SDL_Rect();
    generation_count_rect = SDL_Rect();
    dig_rect = std::vector<SDL_Rect>(40, SDL_Rect());
    confirm_rect = SDL_Rect();
    dig_max_width = 0;

    menu_bg = SDL_Rect();
    generation_count_bg = SDL_Rect();
    confirm_bg = SDL_Rect();

    menu_color = {255, 255, 255, 255};
    generation_count_color = {255, 255, 255, 255};
    dig_color = {255, 255, 255, 255};
    confirm_color = {255, 255, 255, 255};

    menu_bg_color = {0x80, 0x80, 0x80, 0xFF};
    generation_count_bg_color = {0x80, 0x80, 0x80, 0xFF};
    confirm_bg_color = {0x80, 0x80, 0x80, 0xFF};

    paused = false;
}

void SceneTrainingBR::render(SDL_Renderer* rend){
    int gen = std::max(0, std::min(9999, generation));
    // int gen = 152;

    SDL_SetRenderDrawColor(rend, menu_bg_color.r, menu_bg_color.g, menu_bg_color.b, menu_bg_color.a);
    SDL_RenderFillRect(rend, &menu_bg);
    SDL_SetRenderDrawColor(rend, menu_color.r, menu_color.g, menu_color.b, menu_color.a);
    SDL_RenderCopy(rend, menu_tex, NULL, &menu_rect);

    SDL_SetRenderDrawColor(rend, generation_count_bg_color.r, generation_count_bg_color.g, generation_count_bg_color.b, generation_count_bg_color.a);
    SDL_RenderFillRect(rend, &generation_count_bg);
    SDL_SetRenderDrawColor(rend, generation_count_color.r, generation_count_color.g, generation_count_color.b, generation_count_color.a);
    SDL_RenderCopy(rend, generation_count_tex, NULL, &generation_count_rect);

    int digits = gen == 0 ? 1 : (int)std::log10((double)gen) + 1;
    for(int i = digits-1; i >= 0; i--){
        int d = (gen / (int)std::pow(10, i)) % 10;
        SDL_RenderCopy(rend, dig_tex[d], NULL, &dig_rect[digits - i - 1]);
    }

    SDL_SetRenderDrawColor(rend, confirm_bg_color.r, confirm_bg_color.g, confirm_bg_color.b, confirm_bg_color.a);
    SDL_RenderFillRect(rend, &confirm_bg);
    SDL_SetRenderDrawColor(rend, confirm_color.r, confirm_color.g, confirm_color.b, confirm_color.a);
    SDL_RenderCopy(rend, confirm_tex, NULL, &confirm_rect);
}


void SceneTrainingBR::init(int r, int c, int count, int s_w, int s_h, int depth, SDL_Renderer* rend, std::vector<int> seq, PieceRandomizer pr, int gen){

    generation = gen;

    TTF_Font* font = TTF_OpenFont("HunDIN1451.ttf", 20);
    int menu_height = 70;
    int margin_hor_menu = 30;
    int margin_ver_menu = 5;
    int content_height = menu_height - 2*margin_ver_menu;

    int menu_width = 200;
    int menu_margin_h = 10;
    int menu_margin_v = 5;

    int generation_count_width = 600;
    int generation_margin_h = 10;
    int generation_margin_v = 5;
    int generation_x = (s_w - generation_count_width) / 2;

    int confirm_width = 200;
    int confirm_margin_h = 10;
    int confirm_margin_v = 5;


    //init digits
    SDL_Color color = {255, 255, 255, 255};
    for(int i = 0; i < 10; i++){
        SDL_Surface* surf = TTF_RenderText_Solid(font, std::to_string(i).c_str(), color);
        dig_tex[i] = SDL_CreateTextureFromSurface(rend, surf);
        dig_rect[i].w = surf->w;
        dig_rect[i].h = surf->h;
        dig_max_width = std::max(dig_max_width, dig_rect[i].w);

        SDL_FreeSurface(surf);
    }

    //init menu
    SDL_Surface* surf = TTF_RenderText_Solid(font, "Cancel", color);
    menu_tex = SDL_CreateTextureFromSurface(rend, surf);
    menu_rect.w = surf->w;
    menu_rect.h = surf->h;

    SDL_FreeSurface(surf);

    menu_bg.x = margin_hor_menu;
    menu_bg.y = margin_ver_menu;
    menu_bg.w = menu_width;
    menu_bg.h = content_height;

    menu_rect.x = menu_bg.x + (menu_bg.w - menu_rect.w) / 2;
    menu_rect.y = menu_bg.y + (menu_bg.h - menu_rect.h) / 2;

    //init generation count
    surf = TTF_RenderText_Solid(font, "Generation: ", color);
    generation_count_tex = SDL_CreateTextureFromSurface(rend, surf);
    generation_count_rect.w = surf->w;
    generation_count_rect.h = surf->h;

    SDL_FreeSurface(surf);

    generation_count_bg.x = generation_x;
    generation_count_bg.y = margin_ver_menu;
    generation_count_bg.w = generation_count_width;
    generation_count_bg.h = content_height;

    int dig_space = 4 * dig_max_width;

    generation_count_rect.x = generation_count_bg.x + (generation_count_bg.w - generation_count_rect.w - dig_space) / 2;
    generation_count_rect.y = generation_count_bg.y + (generation_count_bg.h - generation_count_rect.h) / 2;

    for(int i = 0; i < 40; i++){
        dig_rect[i].x = generation_count_rect.x + generation_count_rect.w + (i % 10) * dig_max_width;
        dig_rect[i].y = generation_count_rect.y;
    }

    //init confirm
    surf = TTF_RenderText_Solid(font, "Confirm", color);
    confirm_tex = SDL_CreateTextureFromSurface(rend, surf);
    confirm_rect.w = surf->w;
    confirm_rect.h = surf->h;

    SDL_FreeSurface(surf);

    confirm_bg.x = s_w - margin_hor_menu - confirm_width;
    confirm_bg.y = margin_ver_menu;
    confirm_bg.w = confirm_width;
    confirm_bg.h = content_height;

    confirm_rect.x = confirm_bg.x + (confirm_bg.w - confirm_rect.w) / 2;
    confirm_rect.y = confirm_bg.y + (confirm_bg.h - confirm_rect.h) / 2;

    TTF_CloseFont(font);

    R = r;
    C = c;
    COUNT = count;

    int w = (int)(s_w / c);
    int h = (int)((s_h-menu_height) / r);

    int predict_tile_size = std::min((int)(w-40)/20, (int)(h)/24);
    std::cout << "Predicted tile size: " << predict_tile_size << std::endl;

    assert(predict_tile_size >= 5);

    AIGAME_L = std::vector<AIGame>(count);
    gmassoc = std::vector<int>(count, 0);
    for(int i = 0; i < count; i++){
        AIGAME_L[i] = AIGame(pr);
        std::vector<int> cross = crossover(seq, seq);
        AIGAME_L[i].set_genetic_sequence(cross);
        AIGAME_L[i].pps_cd = 0;
        AIGAME_L[i].set_depth(depth);
    }
    AIGAME_L[0].set_genetic_sequence(seq);
    
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            AIGAME_L[i*c+j].load_coords(rend, j*w, i*h + menu_height, predict_tile_size, w, h);
        }
    }

    for(int i = 0; i < count; i += 2){
        AIGAME_L[i].set_enemy_game(&AIGAME_L[i+1]);
        AIGAME_L[i+1].set_enemy_game(&AIGAME_L[i]);
        gmassoc[i] = i+1;
        gmassoc[i+1] = i;
    }

    death_count = 0;
    death_status = std::vector<bool>(count, false);
    mother_genetic_sequence = seq;
    father_genetic_sequence = seq;

    paused = false;
}

void SceneTrainingBR::update(SDL_Renderer* rend, int dt, SDL_Event* event, bool* isRunning, int* state){

    
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            *isRunning = false;
        }

        if (event->type == SDL_MOUSEBUTTONDOWN) {
            int x = event->button.x;
            int y = event->button.y;

            if(x >= menu_bg.x && x <= menu_bg.x + menu_bg.w && y >= menu_bg.y && y <= menu_bg.y + menu_bg.h){
                *state = TRAIN_CANCEL;
            }

            if(x >= confirm_bg.x && x <= confirm_bg.x + confirm_bg.w && y >= confirm_bg.y && y <= confirm_bg.y + confirm_bg.h){
                *state = TRAIN_CONFIRM;
            }
        }

        // get mouse hover
        if (event->type == SDL_MOUSEMOTION) {
            int x = event->motion.x;
            int y = event->motion.y;

            if(x >= menu_bg.x && x <= menu_bg.x + menu_bg.w && y >= menu_bg.y && y <= menu_bg.y + menu_bg.h){
                menu_bg_color = {0x60, 0x60, 0x60, 0xFF};
            }else{
                menu_bg_color = {0x80, 0x80, 0x80, 0xFF};
            }

            if(x >= confirm_bg.x && x <= confirm_bg.x + confirm_bg.w && y >= confirm_bg.y && y <= confirm_bg.y + confirm_bg.h){
                confirm_bg_color = {0x60, 0x60, 0x60, 0xFF};
            }else{
                confirm_bg_color = {0x80, 0x80, 0x80, 0xFF};
            }
        }

        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.sym == SDLK_ESCAPE) {
                *state = TRAIN_CANCEL;
                return;
            }

            if (event->key.keysym.sym == SDLK_SPACE) {
                paused = !paused;
            }
        }
    }

    double begin = clock();

    render(rend);

    if(paused){
        for(int i = 0; i < COUNT; i++){
            AIGAME_L[i].draw(rend);
        }
        return;
    }

    for(int i = 0; i < COUNT; i++){
        AIGAME_L[i].updateDeltaTime(dt, rend, true, &this->diag);
    }

    for(int i = 0; i < COUNT; i++) if(!AIGAME_L[i].alive && !death_status[i]){
        death_status[i] = true;
        death_count += 1;
        gmassoc[gmassoc[i]] = -1;
        gmassoc[i] = -1;

        if(death_count == COUNT-1){
            father_genetic_sequence = AIGAME_L[i].genetic_sequence;

            for(int j = 0; j < COUNT; j++) if(!death_status[j]){
                mother_genetic_sequence = AIGAME_L[j].genetic_sequence;

                generation += 1;

                int prand_seed = std::rand() % 100;

                for(int k = 0; k < COUNT; k++){
                    AIGAME_L[k].reset(prand_seed);
                    std::vector<int> cross = crossover(mother_genetic_sequence, mother_genetic_sequence);
                    AIGAME_L[k].set_genetic_sequence(cross);
                }

                AIGAME_L[0].set_genetic_sequence(mother_genetic_sequence);
                // AIGAME_L[COUNT-1].set_genetic_sequence(father_genetic_sequence);

                std::cout<<"MOTHER: ";
                for(int k = 0; k < mother_genetic_sequence.size(); k++){
                    std::cout<<mother_genetic_sequence[k]<<" ";
                }
                std::cout<<std::endl;

                death_count = 0;
                death_status = std::vector<bool>(COUNT, false);

                for(int k = 0; k < COUNT; k += 2){
                    AIGAME_L[k].set_enemy_game(&AIGAME_L[k+1]);
                    AIGAME_L[k+1].set_enemy_game(&AIGAME_L[k]);
                    gmassoc[k] = k+1;
                    gmassoc[k+1] = k;
                }
                
                return;
            }   
        }

        
    }

    // check limbo
    for(int i = 0; i < COUNT; i++)      if(AIGAME_L[i].limbo){
        for(int j = 0; j < COUNT; j++)  if(i != j && AIGAME_L[j].limbo){
            int prand_seed = std::rand() % 100;
            AIGAME_L[i].reset(prand_seed);
            AIGAME_L[j].reset(prand_seed);
            AIGAME_L[i].set_enemy_game(&AIGAME_L[j]);
            AIGAME_L[j].set_enemy_game(&AIGAME_L[i]);
            gmassoc[i] = j;
            gmassoc[j] = i;
            break;
        }
    }

    t_time += (double)(clock() - begin) / CLOCKS_PER_SEC;

    if(t_time > 20){
        t_time = 0;
        std::cout<<"TIME: "<<t_time<<std::endl;
        std::cout<<"DRAW TIME: "<<this->diag[DIA_DRAW]<<std::endl;
        // std::cout<<"EVAL TIME: "<<this->diag[DIA_EVAL]<<std::endl;
        std::cout<<"CHOOSE TIME: "<<this->diag[DIA_CHOOSE]<<std::endl;
        std::cout<<"d2 TIME: "<<this->diag[4]<<std::endl;
        this->diag[DIA_DRAW] = 0;
        this->diag[DIA_EVAL] = 0;
        this->diag[DIA_CHOOSE] = 0;
    }
}