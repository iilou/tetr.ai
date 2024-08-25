#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

#include "Game.h"
#include "Piece.h"
#include "Collision.h"
#include "constants.h"
#include "AIHeuristics.h"
#include "AIModel.h"
#include "SceneTrainingBR.h"
#include "SceneTrainingSC.h"
#include "SceneTrainingSV.h"
#include "SceneBattle.h"
#include "SceneMenu.h"
#include "SceneSettings.h"
#include "SCENENAV.h"

void read_settings(SceneSettings* scene_settings, int* fps, int* pps, int* ai_depth, int* init_move_cd, int* move_cd, int* soft_drop_cd, int* population_size){
    // call get_option_val from scene_settings pointer
    *fps = scene_settings->get_option_val(0);
    *pps = scene_settings->get_option_val(1);
    *ai_depth = scene_settings->get_option_val(2);
    *init_move_cd = scene_settings->get_option_val(3);
    *move_cd = scene_settings->get_option_val(4);
    *soft_drop_cd = scene_settings->get_option_val(5);
    *population_size = scene_settings->get_option_val(6);
}

int main(int argc, char *argv[])
{
    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 720;
 
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    if (TTF_Init() != 0) {
        printf("error initializing TTF: %s\n", TTF_GetError());
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

    std::vector<int> custom_gene_seq = get_default(); // custom gene sequence
    // std::vector<int> custom_gene_seq = get_prebuilt(); // custom gene sequence
    std::vector<int> prebuilt_gene_seq = get_prebuilt();

    std::cout<<"LOADING"<<std::endl;

    int state=0;
    int prev_state=0;
    bool switch_state = false;

    
    SceneTrainingBR scene_train_br = SceneTrainingBR();
    SceneTrainingSC scene_train_sc = SceneTrainingSC();
    SceneTrainingSV scene_train_sv = SceneTrainingSV();
    SceneSettings scene_settings = SceneSettings();
    SceneBattle scene_battle = SceneBattle();
    SceneMenu scene_menu = SceneMenu();

    int prev_generation = 0;

    


    int fps = 120;
    int pps = 5;
    int ai_depth = 1;
    int init_move_cd = 100;
    int move_cd = 0;
    int soft_drop_cd = 0;
    int ps = 0;



    int cntl[3] = {2, 8, 18};
    int rl[3] = {1, 2, 3};
    int cl[3] = {2, 4, 6};

    int width = 1280;
    int height = 720;

    scene_menu.init(width, height, renderer);
    scene_settings.pre_init_options();
    read_settings(&scene_settings, &fps, &pps, &ai_depth, &init_move_cd, &move_cd, &soft_drop_cd, &ps);

    std::cout<<"SETTINGS: "<<fps<<" "<<pps<<" "<<ai_depth<<" "<<init_move_cd<<" "<<move_cd<<" "<<soft_drop_cd<<" "<<ps<<std::endl;

    std::cout<<"START"<<std::endl;

    while (isRunning) {
        prev_state = state;
        // state = 0;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if(prev_state==MENU_DEFAULT || prev_state==MENU_BATTLE_SELECT || prev_state==MENU_TRAIN_SELECT){
            scene_menu.render(renderer, &event, &isRunning, &state);

            // NAVIGATE TO [INSERT SCENE] FROM MENU
            if(state == MENU_TRAIN_BATTLE_ROYALE){ // TRAINING BATTLE ROYALE
                scene_train_br.init(rl[ps], cl[ps], cntl[ps], width, height, ai_depth, renderer, custom_gene_seq, PieceRandomizer(100), prev_generation);
            }

            else if (state == MENU_TRAIN_SPRINT_CHALLENGE){ // TRAINING SPRINT CHALLENGE
                scene_train_sc.init(rl[ps], cl[ps], cntl[ps], width, height, ai_depth, renderer, custom_gene_seq, PieceRandomizer(100), prev_generation);
            }

            else if(state == MENU_TRAIN_SURVIVAL_CHALLENGE){ // TRAINING SURVIVAL CHALLENGE
                scene_train_sv.init(rl[ps], cl[ps], cntl[ps], width, height, ai_depth, renderer, custom_gene_seq, PieceRandomizer(100), prev_generation);
            }

            else if(state == MENU_BATTLE_CUSTOM_VS_PREBUILT || state == MENU_BATTLE_PLAYER_VS_CUSTOM || state == MENU_BATTLE_PLAYER_VS_PREBUILT){ // BATTLE
                scene_battle.init(width, height, renderer, state, custom_gene_seq, prebuilt_gene_seq, pps, ai_depth, init_move_cd, move_cd, soft_drop_cd);
            }

            else if(state == MENU_SETTINGS){
                scene_settings.init(renderer, width, height);
            }   
        }

        else if(prev_state==MENU_BATTLE_CUSTOM_VS_PREBUILT || prev_state==MENU_BATTLE_PLAYER_VS_CUSTOM || prev_state==MENU_BATTLE_PLAYER_VS_PREBUILT){
            scene_battle.updateDeltaTime(renderer, &event, 1000/fps, &isRunning, &state);
        }

        else if(prev_state==MENU_TRAIN_BATTLE_ROYALE){
            scene_train_br.update(renderer, 1000/fps, &event, &isRunning, &state);
            if(state == TRAIN_CANCEL){ 
                scene_train_br.generation = prev_generation;
                state = MENU_DEFAULT;
            } else if(state == TRAIN_CONFIRM){
                custom_gene_seq = scene_train_br.mother_genetic_sequence;
                prev_generation = scene_train_br.generation;
                state = MENU_DEFAULT;
            }
        }

        else if(prev_state==MENU_TRAIN_SPRINT_CHALLENGE){
            scene_train_sc.update(renderer, 1000/fps, &event, &isRunning, &state);
            if(state == TRAIN_CANCEL){ 
                scene_train_sc.generation = prev_generation;
                state = MENU_DEFAULT;
            } else if(state == TRAIN_CONFIRM){
                custom_gene_seq = scene_train_sc.mother_genetic_sequence;
                prev_generation = scene_train_sc.generation;
                state = MENU_DEFAULT;
            }
        }

        else if(prev_state==MENU_TRAIN_SURVIVAL_CHALLENGE){
            scene_train_sv.update(renderer, 1000/fps, &event, &isRunning, &state);
            if(state == TRAIN_CANCEL){ 
                scene_train_sv.generation = prev_generation;
                state = MENU_DEFAULT;
            } else if(state == TRAIN_CONFIRM){
                custom_gene_seq = scene_train_sv.mother_genetic_sequence;
                prev_generation = scene_train_sv.generation;
                state = MENU_DEFAULT;
            }
        }

        else if(prev_state==MENU_SETTINGS){
            scene_settings.render(renderer, &event, &isRunning, &state);

            if(state == MENU_DEFAULT){
                read_settings(&scene_settings, &fps, &pps, &ai_depth, &init_move_cd, &move_cd, &soft_drop_cd, &ps);
            }
        }

        // switch_state = state!=0;

        // if(switch_state){
        //     int exit_code = state;
        //     if(exit_
        
        // scene_train_sc.update(renderer, 8);

        SDL_RenderPresent(renderer);

        SDL_Delay(1000/fps);
    }

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
