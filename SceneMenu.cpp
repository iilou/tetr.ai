#include <iostream>
#include <vector>

#include "SceneMenu.h"
#include "Game.h"
#include "AIHeuristics.h"
#include "AIModel.h"
#include "SCENENAV.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"


SceneMenu::SceneMenu(){
    menu_item_texts = std::vector<SDL_Texture*>(menu_items.size());
    menu_item_texts_highlighted = std::vector<SDL_Texture*>(menu_items.size());
    menu_item_text_rects = std::vector<SDL_Rect>(menu_items.size());
    menu_item_highlighted = std::vector<bool>(menu_items.size());

    battle_menu_item_texts = std::vector<SDL_Texture*>(battle_menu_items.size());
    battle_menu_item_text_rects = std::vector<SDL_Rect>(battle_menu_items.size());
    battle_menu_item_rects = std::vector<SDL_Rect>(battle_menu_items.size());
    battle_menu_item_highlighted = std::vector<bool>(battle_menu_items.size());

    training_menu_item_texts = std::vector<SDL_Texture*>(training_menu_items.size());
    training_menu_item_text_rects = std::vector<SDL_Rect>(training_menu_items.size());
    training_menu_item_rects = std::vector<SDL_Rect>(training_menu_items.size());
    training_menu_item_highlighted = std::vector<bool>(training_menu_items.size());
}



void SceneMenu::init(int areaWidth, int areaHeight, SDL_Renderer* renderer){
    TTF_Font* font = TTF_OpenFont("HunDIN1451.ttf", 24);
    if (font == NULL){
        std::cout<<"Error loading font"<<std::endl;
    }

    int title_box_width = 840;
    int title_box_height = 180;
    int title_menu_margin = 80;
    int menu_item_width = 300;
    int menu_item_height = 45;
    int menu_item_spacing = 15;
    

    assert(menu_items.size() * menu_item_height + (menu_items.size()-1) * menu_item_spacing + title_box_height + title_menu_margin < areaHeight);

    int main_interface_height = menu_items.size() * menu_item_height + (menu_items.size()-1) * menu_item_spacing + title_box_height + title_menu_margin;
    int main_interface_y = (areaHeight - main_interface_height) / 2;

    font = TTF_OpenFont("HunDIN1451.ttf", 112);
    SDL_Surface* title_surface = TTF_RenderText_Solid(font, title_str, title_color);
    title_text = SDL_CreateTextureFromSurface(renderer, title_surface);
    title_text_rect = SDL_Rect{0, 0, title_surface->w, title_surface->h};
    title_text_rect.x = (areaWidth - title_surface->w) / 2;
    title_text_rect.y = main_interface_y + (title_box_height - title_surface->h) / 2 + 15; // offset cause idk???
    title_rect = SDL_Rect{(areaWidth - title_box_width) / 2, main_interface_y, title_box_width, title_box_height}; 
    SDL_FreeSurface(title_surface);

    std::cout<<"title init"<<std::endl;

    font = TTF_OpenFont("HunDIN1451.ttf", 32);
    SDL_Surface* menu_item_surface;
    for (int i=0; i<menu_items.size(); i++){
        menu_item_surface = TTF_RenderText_Solid(font, menu_items[i], menu_color);
        menu_item_texts[i] = SDL_CreateTextureFromSurface(renderer, menu_item_surface);
        SDL_FreeSurface(menu_item_surface);
        menu_item_surface = TTF_RenderText_Solid(font, menu_items[i], menu_highlight_color);
        menu_item_texts_highlighted[i] = SDL_CreateTextureFromSurface(renderer, menu_item_surface);
        menu_item_text_rects[i] = SDL_Rect{0, 0, menu_item_surface->w, menu_item_surface->h};
        menu_item_text_rects[i].x = (areaWidth - menu_item_surface->w) / 2;
        menu_item_text_rects[i].y = main_interface_y + title_box_height + title_menu_margin + i * (menu_item_height + menu_item_spacing) + (menu_item_height - menu_item_surface->h) / 2;
        SDL_FreeSurface(menu_item_surface);
    }

    //exit button
    int exit_box_width = 200;
    int exit_box_height = 40;
    font = TTF_OpenFont("HunDIN1451.ttf", 24);
    SDL_Surface* battle_exit_surface = TTF_RenderText_Solid(font, menu_exit_str, menu_color);
    battle_exit_text = SDL_CreateTextureFromSurface(renderer, battle_exit_surface);
    battle_exit_text_rect = SDL_Rect{0, 0, battle_exit_surface->w, battle_exit_surface->h};
    battle_exit_text_rect.x = 15 + (exit_box_width - battle_exit_surface->w) / 2;
    battle_exit_text_rect.y = 15 + (exit_box_height - battle_exit_surface->h) / 2;
    battle_exit_rect = SDL_Rect{15, 15, exit_box_width, exit_box_height};
    SDL_FreeSurface(battle_exit_surface);



    // battle menu
    int battle_menu_item_width = 700;
    int battle_menu_item_height = 80;
    int battle_menu_item_spacing = 20;
    int battle_y = (areaHeight - (battle_menu_items.size() * battle_menu_item_height + (battle_menu_items.size()-1) * battle_menu_item_spacing)) / 2;
    font = TTF_OpenFont("HunDIN1451.ttf", 32);
    SDL_Surface* battle_menu_item_surface;
    for(int i = 0; i < battle_menu_items.size(); i++){
        battle_menu_item_surface = TTF_RenderText_Solid(font, battle_menu_items[i], menu_color);
        battle_menu_item_texts[i] = SDL_CreateTextureFromSurface(renderer, battle_menu_item_surface);
        battle_menu_item_text_rects[i] = SDL_Rect{0, 0, battle_menu_item_surface->w, battle_menu_item_surface->h};
        battle_menu_item_text_rects[i].x = (areaWidth - battle_menu_item_surface->w) / 2;
        battle_menu_item_text_rects[i].y = battle_y + i * (battle_menu_item_height + battle_menu_item_spacing) + (battle_menu_item_height - battle_menu_item_surface->h) / 2;
        battle_menu_item_rects[i] = SDL_Rect{(areaWidth - battle_menu_item_width) / 2, battle_y + i * (battle_menu_item_height + battle_menu_item_spacing), battle_menu_item_width, battle_menu_item_height};
        SDL_FreeSurface(battle_menu_item_surface);
    }


    // training menu
    int training_menu_item_width = 700;
    int training_menu_item_height = 80;
    int training_menu_item_spacing = 20;
    int training_y = (areaHeight - (training_menu_items.size() * training_menu_item_height + (training_menu_items.size()-1) * training_menu_item_spacing)) / 2;
    font = TTF_OpenFont("HunDIN1451.ttf", 32);
    SDL_Surface* training_menu_item_surface;
    for(int i = 0; i < training_menu_items.size(); i++){
        training_menu_item_surface = TTF_RenderText_Solid(font, training_menu_items[i], menu_color);
        training_menu_item_texts[i] = SDL_CreateTextureFromSurface(renderer, training_menu_item_surface);
        training_menu_item_text_rects[i] = SDL_Rect{0, 0, training_menu_item_surface->w, training_menu_item_surface->h};
        training_menu_item_text_rects[i].x = (areaWidth - training_menu_item_surface->w) / 2;
        training_menu_item_text_rects[i].y = training_y + i * (training_menu_item_height + training_menu_item_spacing) + (training_menu_item_height - training_menu_item_surface->h) / 2;
        training_menu_item_rects[i] = SDL_Rect{(areaWidth - training_menu_item_width) / 2, training_y + i * (training_menu_item_height + training_menu_item_spacing), training_menu_item_width, training_menu_item_height};
        SDL_FreeSurface(training_menu_item_surface);
    }

}

void SceneMenu::render(SDL_Renderer* renderer, SDL_Event* event, bool* isRunning, int* state){
    
    while(SDL_PollEvent(event)){
        if (event->type == SDL_QUIT){
            *isRunning = false;
        }

        // click
        if (event->type == SDL_MOUSEBUTTONDOWN){
            int x, y;
            SDL_GetMouseState(&x, &y);
            if(*state==MENU_DEFAULT) for (int i=0; i<menu_items.size(); i++){ // main menu
                if (x > menu_item_text_rects[i].x && x < menu_item_text_rects[i].x + menu_item_text_rects[i].w && y > menu_item_text_rects[i].y && y < menu_item_text_rects[i].y + menu_item_text_rects[i].h){
                    if (i == 0){
                        *state = MENU_BATTLE_SELECT;
                        return;
                    } else if (i == 1){
                        *state = MENU_TRAIN_SELECT;
                        return;
                    } else if (i == 2){
                        // settings
                        *state = MENU_SETTINGS;
                        return;
                    } else if (i == 3){
                        // quit
                        *isRunning = false;
                    }
                }
            }
            else if (*state==MENU_BATTLE_SELECT){ // battle menu
                for (int i=0; i<battle_menu_items.size(); i++){
                    if (x > battle_menu_item_rects[i].x && x < battle_menu_item_rects[i].x + battle_menu_item_rects[i].w && y > battle_menu_item_rects[i].y && y < battle_menu_item_rects[i].y + battle_menu_item_rects[i].h){
                        if (i == 0){
                            *state = MENU_BATTLE_PLAYER_VS_CUSTOM;
                            return;
                        } 
                        else if (i == 1){
                            *state = MENU_BATTLE_PLAYER_VS_PREBUILT;
                            return;
                        } else if (i == 2){
                            *state = MENU_BATTLE_CUSTOM_VS_PREBUILT;
                            return;
                        }
                    }
                }

                if (x > battle_exit_rect.x && x < battle_exit_rect.x + battle_exit_rect.w && y > battle_exit_rect.y && y < battle_exit_rect.y + battle_exit_rect.h){
                    *state = MENU_DEFAULT;
                    return;
                }
            }
            else if (*state==MENU_TRAIN_SELECT){ // training menu
                for (int i=0; i<training_menu_items.size(); i++){
                    if (x > training_menu_item_rects[i].x && x < training_menu_item_rects[i].x + training_menu_item_rects[i].w && y > training_menu_item_rects[i].y && y < training_menu_item_rects[i].y + training_menu_item_rects[i].h){
                        if (i == 0){
                            *state = MENU_TRAIN_BATTLE_ROYALE;
                            return;
                        } else if (i == 1){
                            *state = MENU_TRAIN_SPRINT_CHALLENGE;
                            return;
                        } else if (i == 2){
                            *state = MENU_TRAIN_SURVIVAL_CHALLENGE;
                            return;
                        }
                    }
                }

                if (x > battle_exit_rect.x && x < battle_exit_rect.x + battle_exit_rect.w && y > battle_exit_rect.y && y < battle_exit_rect.y + battle_exit_rect.h){
                    *state = MENU_DEFAULT;
                    return;
                }
            }
        }


        //hover
        if(event->type == SDL_MOUSEMOTION){
            int x, y;
            SDL_GetMouseState(&x, &y);

            if(*state==MENU_DEFAULT) for (int i=0; i<menu_items.size(); i++){
                // ----------------------------------------------------------------- HARD CODE THIS PART -----------------------------------------------------------------
                if (y > menu_item_text_rects[i].y - 5 && y < menu_item_text_rects[i].y + menu_item_text_rects[i].h + 5){
                    menu_item_highlighted[i] = true;
                } else {
                    menu_item_highlighted[i] = false;
                }
            }

            else if(*state==MENU_BATTLE_SELECT){
                for (int i=0; i<battle_menu_items.size(); i++){
                    if (x > battle_menu_item_rects[i].x && x < battle_menu_item_rects[i].x + battle_menu_item_rects[i].w && y > battle_menu_item_rects[i].y && y < battle_menu_item_rects[i].y + battle_menu_item_rects[i].h){
                        battle_menu_item_highlighted[i] = true;
                    } else {
                        battle_menu_item_highlighted[i] = false;
                    }
                }
            }

            else if(*state==MENU_TRAIN_SELECT){
                for (int i=0; i<training_menu_items.size(); i++){
                    if (x > training_menu_item_rects[i].x && x < training_menu_item_rects[i].x + training_menu_item_rects[i].w && y > training_menu_item_rects[i].y && y < training_menu_item_rects[i].y + training_menu_item_rects[i].h){
                        training_menu_item_highlighted[i] = true;
                    } else {
                        training_menu_item_highlighted[i] = false;
                    }
                }
            }
        }
    }

    if(*state==MENU_DEFAULT) {
        SDL_RenderCopy(renderer, title_text, NULL, &title_text_rect);
        SDL_SetRenderDrawColor(renderer, title_color.r, title_color.g, title_color.b, title_color.a);
        SDL_RenderDrawRect(renderer, &title_rect);

        for (int i=0; i<menu_items.size(); i++){
            if (menu_item_highlighted[i]){
                SDL_RenderCopy(renderer, menu_item_texts_highlighted[i], NULL, &menu_item_text_rects[i]);
            } else {
                SDL_RenderCopy(renderer, menu_item_texts[i], NULL, &menu_item_text_rects[i]);
            }
        }
    }

    else if(*state==MENU_BATTLE_SELECT){
        SDL_RenderCopy(renderer, battle_exit_text, NULL, &battle_exit_text_rect);
        SDL_SetRenderDrawColor(renderer, battle_item_text_color.r, battle_item_text_color.g, battle_item_text_color.b, battle_item_text_color.a);
        for (int i=0; i<battle_menu_items.size(); i++){
            SDL_RenderCopy(renderer, battle_menu_item_texts[i], NULL, &battle_menu_item_text_rects[i]);
            if(battle_menu_item_highlighted[i]){
                SDL_RenderDrawRect(renderer, &battle_menu_item_rects[i]);
            }
        }
    }

    else if(*state==MENU_TRAIN_SELECT){
        SDL_RenderCopy(renderer, battle_exit_text, NULL, &battle_exit_text_rect);
        SDL_SetRenderDrawColor(renderer, battle_item_text_color.r, battle_item_text_color.g, battle_item_text_color.b, battle_item_text_color.a);
        for (int i=0; i<training_menu_items.size(); i++){
            SDL_RenderCopy(renderer, training_menu_item_texts[i], NULL, &training_menu_item_text_rects[i]);
            if(training_menu_item_highlighted[i]){
                SDL_RenderDrawRect(renderer, &training_menu_item_rects[i]);
            }
        }
    }

}