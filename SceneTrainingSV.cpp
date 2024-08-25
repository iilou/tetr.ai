#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <string>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

#include "Game.h"
#include "Piece.h"
#include "Collision.h"
#include "constants.h"
#include "AIHeuristics.h"
#include "AIModel.h"
#include "SceneTrainingSV.h"
#include "SCENENAV.h"

SceneTrainingSV::SceneTrainingSV(){
    R = -1;
    C = -1;
    COUNT = -1;
    death_count = 0;

    AIGAME_L = std::vector<AIGame>(10);
    line_timer = 0;
    line_timer_dur = -1; // 1 second
    line_timer_factor = 0.96;
    line_timer_initial = 1000;
    score_threshold = 999; // max score
    end_timer = 0;
    end_timer_dur = 60000; // 1 minute

    grid = std::vector<std::vector<int>>(24, std::vector<int>(10, 0));
    genes = get_default();

    death_status = std::vector<bool>(10, false);
    mother_genetic_sequence = std::vector<int>();
    father_genetic_sequence = std::vector<int>();

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
};

void SceneTrainingSV::init(int r, int c, int count, int s_w, int s_h, int depth, SDL_Renderer* rend, std::vector<int> seq, PieceRandomizer pr, int gen){
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

    death_count = 0;
    death_status = std::vector<bool>(count, false);
    mother_genetic_sequence = seq;
    father_genetic_sequence = seq;

    paused = false;

    line_timer = line_timer_initial;
    line_timer_dur = line_timer_initial;
    end_timer = end_timer_dur;

};

void SceneTrainingSV::render(SDL_Renderer* rend){
    int gen = std::max(0, std::min(9999, generation));

    SDL_SetRenderDrawColor(rend, menu_bg_color.r, menu_bg_color.g, menu_bg_color.b, menu_bg_color.a);
    SDL_RenderFillRect(rend, &menu_bg);
    SDL_SetRenderDrawColor(rend, menu_color.r, menu_color.g, menu_color.b, menu_color.a);
    SDL_RenderCopy(rend, menu_tex, NULL, &menu_rect);

    SDL_SetRenderDrawColor(rend, generation_count_bg_color.r, generation_count_bg_color.g, generation_count_bg_color.b, generation_count_bg_color.a);
    SDL_RenderFillRect(rend, &generation_count_bg);
    SDL_SetRenderDrawColor(rend, generation_count_color.r, generation_count_color.g, generation_count_color.b, generation_count_color.a);
    SDL_RenderCopy(rend, generation_count_tex, NULL, &generation_count_rect);
    int digits = gen == 0 ? 1 : (int)std::log10((double)gen) + 1;
    for(int i = 0; i < digits; i++){
        SDL_RenderCopy(rend, dig_tex[gen % 10], NULL, &dig_rect[i]);
        gen /= 10;
    }

    SDL_SetRenderDrawColor(rend, confirm_bg_color.r, confirm_bg_color.g, confirm_bg_color.b, confirm_bg_color.a);
    SDL_RenderFillRect(rend, &confirm_bg);
    SDL_SetRenderDrawColor(rend, confirm_color.r, confirm_color.g, confirm_color.b, confirm_color.a);
    SDL_RenderCopy(rend, confirm_tex, NULL, &confirm_rect);
}

void SceneTrainingSV::end_game(){
    std::cout<<"SceneTrainingSV end_game"<<std::endl;
    for(int i = 0; i < mother_genetic_sequence.size(); i++){
        std::cout<<mother_genetic_sequence[i]<<" ";
    }
    std::cout<<std::endl;

    // end game
    int prandseed = rand() % 100;
    for(int i = 0; i < COUNT; i++){
        AIGAME_L[i].reset();
        std::vector<int> new_seq = crossover(mother_genetic_sequence, mother_genetic_sequence);
        AIGAME_L[i].set_genetic_sequence(new_seq);
        AIGAME_L[i].piece_randomizer.pieceIndex = prandseed;
    }

    AIGAME_L[0].set_genetic_sequence(mother_genetic_sequence);
    // AIGAME_L[COUNT-1].set_genetic_sequence(father_genetic_sequence);

    death_count = 0;
    death_status = std::vector<bool>(COUNT, false);

    line_timer = line_timer_initial;
    line_timer_dur = line_timer_initial;
    end_timer = end_timer_dur;

    generation++;
}

void SceneTrainingSV::update(SDL_Renderer* rend, int dt, SDL_Event* event, bool* isRunning, int* state){

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

    if(paused){
        return;
    }

    render(rend);
    // std::cout<<"SceneTrainingSV update"<<std::endl;
    line_timer -= dt;

    // send line
    if(line_timer <= 0){
        line_timer = line_timer_dur;
        int lines = std::rand() % 4 + 1;
        for(int i = 0; i < COUNT; i++){
            AIGAME_L[i].on_enemy_line_sent(lines);
        }
        line_timer_dur = (line_timer_dur * line_timer_factor);
    }

    // update all AIGames
    for(int i = 0; i < R; i++){
        for(int j = 0; j < C; j++){
            AIGAME_L[i*C+j].updateDeltaTime(dt, rend);
        }
    }

    // check max score
    for(int i = 0; i < COUNT; i++){
        if(AIGAME_L[i].lifetime_lines_sent >= score_threshold){
            end_timer = -1;
            mother_genetic_sequence = AIGAME_L[i].genetic_sequence;
            int max_score = -1;
            int max_score_index = -1;
            for(int j = 0; j < COUNT; j++){
                if(AIGAME_L[j].lifetime_lines_sent > max_score){
                    max_score = AIGAME_L[j].lifetime_lines_sent;
                    max_score_index = j;
                }
            }
            father_genetic_sequence = AIGAME_L[max_score_index].genetic_sequence;
            end_game();
            return;
        }
    }

    // check death statuses
    for(int i = 0; i < COUNT; i++){
        if(!AIGAME_L[i].alive && !death_status[i]){
            death_status[i] = true;
            death_count++;
            if(death_count == COUNT-1){
                // end game
                end_timer = -1;
                father_genetic_sequence = AIGAME_L[i].genetic_sequence;
                for(int j = 0; j < COUNT; j++){
                    if(!death_status[j]){
                        mother_genetic_sequence = AIGAME_L[j].genetic_sequence;
                        end_game();
                        return;
                    }
                }
            }
        }
    }

    // check end game conditions
}