#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "SceneBattle.h"
#include "Game.h"
#include "SCENENAV.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

void SceneBattle::init(int width, int height, SDL_Renderer* renderer, int mode, std::vector<int> gene_seq_0, std::vector<int> gene_seq_1, int ai_pps, int ai_depth, int initial_move_cd, int move_cd, int soft_drop_cd){

    this->ally_score = 0;
    this->enemy_score = 0;

    PieceRandomizer pr = PieceRandomizer(100);
    ally_ai_game.reset();
    enemy_ai_game.reset();
    ally_player_game.reset();
    ally_player_game.set_cds(initial_move_cd, move_cd, soft_drop_cd);
    ally_ai_game.set_pps(ai_pps);
    ally_ai_game.set_depth(ai_depth);
    enemy_ai_game.set_pps(ai_pps);
    enemy_ai_game.set_depth(ai_depth);

    if(mode == MENU_BATTLE_CUSTOM_VS_PREBUILT){
        ally_ai_game.set_genetic_sequence(gene_seq_0);
        enemy_ai_game.set_genetic_sequence(gene_seq_1);

        ally_ai_game.setPieceRandomizer(pr);
        enemy_ai_game.setPieceRandomizer(pr);

        ally_ai_game.set_enemy_game(&enemy_ai_game);
        enemy_ai_game.set_enemy_game(&ally_ai_game);
    } else if (mode == MENU_BATTLE_PLAYER_VS_CUSTOM){
        enemy_ai_game.set_genetic_sequence(gene_seq_0);

        ally_player_game.setPieceRandomizer(pr);
        enemy_ai_game.setPieceRandomizer(pr);

        ally_player_game.set_enemy_game(&enemy_ai_game);
        enemy_ai_game.set_enemy_game(&ally_player_game);
    } else if (mode == MENU_BATTLE_PLAYER_VS_PREBUILT){
        enemy_ai_game.set_genetic_sequence(gene_seq_1);

        ally_player_game.setPieceRandomizer(pr);
        enemy_ai_game.setPieceRandomizer(pr);

        ally_player_game.set_enemy_game(&enemy_ai_game);
        enemy_ai_game.set_enemy_game(&ally_player_game);
    }

    int back_button_width = 200;
    int back_button_height = 40;

    int title_width = 200;
    int title_height = 80;
    std::pair<int, int> game_dim = dim_from_tile_size(15);
    int game_width = game_dim.first;
    int game_height = game_dim.second;
    int name_height = 40;

    int margin_title_game = 20;
    int margin_game_name = 10;

    assert(height >= title_height + game_height + title_height + margin_title_game + margin_game_name);

    int off_y = (height - title_height - game_height - title_height - margin_title_game - margin_game_name) / 2;

    // Back button
    TTF_Font* font = TTF_OpenFont("HunDIN1451.ttf", 24);
    SDL_Surface* surface = TTF_RenderText_Solid(font, back_button_str, text_color);
    back_button_texture = SDL_CreateTextureFromSurface(renderer, surface);
    back_button_text_rect = {0, 0, surface->w, surface->h};
    back_button_text_rect.x = 15+(back_button_width - surface->w) / 2;
    back_button_text_rect.y = 15+(back_button_height - surface->h) / 2;
    back_button_rect = {15, 15, back_button_width, back_button_height};
    SDL_FreeSurface(surface);


    // Score
    font = TTF_OpenFont("HunDIN1451.ttf", 60);
    surface = TTF_RenderText_Solid(font, score_str, text_color);
    score_texture = SDL_CreateTextureFromSurface(renderer, surface);
    score_rect = {0, 0, surface->w, surface->h};
    score_rect.x = (width - surface->w) / 2;
    score_rect.y = off_y + (title_height - surface->h) / 2;
    SDL_FreeSurface(surface);


    int max_digit_width = 0;
    // score digits
    score_digit_textures.clear();
    score_digit_rects_ally.clear();
    score_digit_rects_enemy.clear();
    for(int i = 0; i < 10; i++){
        surface = TTF_RenderText_Solid(font, std::to_string(i).c_str(), text_color);
        score_digit_textures.push_back(SDL_CreateTextureFromSurface(renderer, surface));
        score_digit_rects_ally.push_back({0, 0, surface->w, surface->h});
        score_digit_rects_enemy.push_back({0, 0, surface->w, surface->h});
        score_digit_rects_ally[i].x = score_rect.x - 10 - surface->w;
        score_digit_rects_enemy[i].x = score_rect.x + score_rect.w + 10;
        score_digit_rects_ally[i].y = off_y + (title_height - surface->h) / 2;
        score_digit_rects_enemy[i].y = off_y + (title_height - surface->h) / 2;
        max_digit_width = std::max(max_digit_width, surface->w);

        SDL_FreeSurface(surface);
    }


    // Game
    int game_off_x = (width - 2*game_width ) / 3;
    // if(ally_ai_game != nullptr){
    //     ally_ai_game->load_coords(renderer, game_off_x, off_y + title_height + margin_title_game, 15, game_width, game_height);
    // }

    // if(enemy_ai_game != nullptr){
    //     enemy_ai_game->load_coords(renderer, game_off_x*2 + game_width, off_y + title_height + margin_title_game, 15, game_width, game_height);
    // }
    if(mode == MENU_BATTLE_CUSTOM_VS_PREBUILT){
        ally_ai_game.load_coords(renderer, game_off_x, off_y + title_height + margin_title_game, 15, game_width, game_height);
        enemy_ai_game.load_coords(renderer, game_off_x*2 + game_width, off_y + title_height + margin_title_game, 15, game_width, game_height);
    } else if (mode == MENU_BATTLE_PLAYER_VS_CUSTOM){
        ally_player_game.load_coords(renderer, game_off_x, off_y + title_height + margin_title_game, 15, game_width, game_height);
        enemy_ai_game.load_coords(renderer, game_off_x*2 + game_width, off_y + title_height + margin_title_game, 15, game_width, game_height);
    } else if (mode == MENU_BATTLE_PLAYER_VS_PREBUILT){
        ally_player_game.load_coords(renderer, game_off_x, off_y + title_height + margin_title_game, 15, game_width, game_height);
        enemy_ai_game.load_coords(renderer, game_off_x*2 + game_width, off_y + title_height + margin_title_game, 15, game_width, game_height);
    }


    // Names
    font = TTF_OpenFont("HunDIN1451.ttf", 24);

    surface = TTF_RenderText_Solid(font, player_name_str, text_color);
    player_name_texture = SDL_CreateTextureFromSurface(renderer, surface);
    player_name_rect_ally = {0, 0, surface->w, surface->h};
    player_name_rect_ally.x = game_off_x + (game_width - surface->w) / 2;
    player_name_rect_ally.y = off_y + title_height + margin_title_game + game_height + margin_game_name;
    SDL_FreeSurface(surface);
    
    surface = TTF_RenderText_Solid(font, cai_name_str, text_color);
    cai_name_texture = SDL_CreateTextureFromSurface(renderer, surface);
    cai_name_rect_ally = {0, 0, surface->w, surface->h};
    cai_name_rect_ally.x = game_off_x + (game_width - surface->w) / 2;
    cai_name_rect_ally.y = off_y + title_height + margin_title_game - margin_game_name - surface->h;
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(font, cai_name_str, text_color);
    cai_name_texture = SDL_CreateTextureFromSurface(renderer, surface);
    cai_name_rect_enemy = {0, 0, surface->w, surface->h};
    cai_name_rect_enemy.x = game_off_x*2 + game_width + (game_width - surface->w) / 2;
    cai_name_rect_enemy.y = off_y + title_height + margin_title_game - margin_game_name - surface->h;
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(font, pai_name_str, text_color);
    pai_name_texture = SDL_CreateTextureFromSurface(renderer, surface);
    pai_name_rect_enemy = {0, 0, surface->w, surface->h};
    pai_name_rect_enemy.x = game_off_x*2 + game_width + (game_width - surface->w) / 2;
    pai_name_rect_enemy.y = off_y + title_height + margin_title_game + game_height + margin_game_name;
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);



}


void SceneBattle::render(SDL_Renderer* renderer, int state){
    if(state == MENU_BATTLE_CUSTOM_VS_PREBUILT){
        ally_ai_game.draw(renderer);
        enemy_ai_game.draw(renderer);
    } else if (state == MENU_BATTLE_PLAYER_VS_CUSTOM){
        ally_player_game.draw(renderer);
        enemy_ai_game.draw(renderer);
    } else if (state == MENU_BATTLE_PLAYER_VS_PREBUILT){
        ally_player_game.draw(renderer);
        enemy_ai_game.draw(renderer);
    }

    SDL_SetRenderDrawColor(renderer, this->text_color.r, this->text_color.g, this->text_color.b, this->text_color.a);
    SDL_RenderDrawRect(renderer, &back_button_rect);
    SDL_RenderCopy(renderer, back_button_texture, NULL, &back_button_text_rect);

    assert(ally_score >= 0 && ally_score < 10);
    assert(enemy_score >= 0 && enemy_score < 10);
    SDL_RenderCopy(renderer, score_texture, NULL, &score_rect);
    SDL_RenderCopy(renderer, score_digit_textures[ally_score], NULL, &score_digit_rects_ally[ally_score]);
    SDL_RenderCopy(renderer, score_digit_textures[enemy_score], NULL, &score_digit_rects_enemy[enemy_score]);

    // SDL_RenderCopy(renderer, player_name_texture, NULL, &player_name_rect_ally);
    // SDL_RenderCopy(renderer, cai_name_texture, NULL, &cai_name_rect_ally);
    // SDL_RenderCopy(renderer, cai_name_texture, NULL, &cai_name_rect_enemy);
    // SDL_RenderCopy(renderer, pai_name_texture, NULL, &pai_name_rect_enemy);
    if(state == MENU_BATTLE_CUSTOM_VS_PREBUILT) SDL_RenderCopy(renderer, cai_name_texture, NULL, &cai_name_rect_ally);
    else SDL_RenderCopy(renderer, player_name_texture, NULL, &player_name_rect_ally);
    if(state == MENU_BATTLE_PLAYER_VS_CUSTOM) SDL_RenderCopy(renderer, cai_name_texture, NULL, &cai_name_rect_enemy);
    else SDL_RenderCopy(renderer, pai_name_texture, NULL, &pai_name_rect_enemy);
}



void SceneBattle::updateDeltaTime(SDL_Renderer* renderer, SDL_Event* event, int dt, bool* isRunning, int* state){

    while(SDL_PollEvent(event)){
        if(event->type == SDL_QUIT){
            *isRunning = false;
        }
        else if(event->type == SDL_KEYDOWN){
            if(event->key.keysym.sym == SDLK_ESCAPE){
                *state = MENU_DEFAULT;
            }
        }
        
        if(event->type == SDL_MOUSEBUTTONDOWN){
            int x, y;
            SDL_GetMouseState(&x, &y);
            if(x >= back_button_rect.x && x <= back_button_rect.x + back_button_rect.w && y >= back_button_rect.y && y <= back_button_rect.y + back_button_rect.h){
                *state = MENU_DEFAULT;
                return;
            }
        }

        if(event->type == SDL_KEYDOWN){
            if(event->key.keysym.sym == SDLK_ESCAPE){
                *state = MENU_DEFAULT;
                return;
            }
            if(*state == MENU_BATTLE_PLAYER_VS_CUSTOM || *state == MENU_BATTLE_PLAYER_VS_PREBUILT){
                ally_player_game.on_key_down(event->key.keysym.sym);
            }
        }

        if(event->type == SDL_KEYUP){
            if(*state == MENU_BATTLE_PLAYER_VS_CUSTOM || *state == MENU_BATTLE_PLAYER_VS_PREBUILT){
                ally_player_game.on_key_up(event->key.keysym.sym);
            }
        }
    }

    if(*state == MENU_DEFAULT){
        return;
    }

    render(renderer, *state);

    if(*state == MENU_BATTLE_CUSTOM_VS_PREBUILT){
        ally_ai_game.updateDeltaTime(dt, renderer);
        enemy_ai_game.updateDeltaTime(dt, renderer);
    } else if (*state == MENU_BATTLE_PLAYER_VS_CUSTOM){
        ally_player_game.updateDeltaTime(dt, renderer);
        enemy_ai_game.updateDeltaTime(dt, renderer);
    } else if (*state == MENU_BATTLE_PLAYER_VS_PREBUILT){
        ally_player_game.updateDeltaTime(dt, renderer);
        enemy_ai_game.updateDeltaTime(dt, renderer);
    }

    if( ( *state == MENU_BATTLE_CUSTOM_VS_PREBUILT && (!ally_ai_game.alive || !enemy_ai_game.alive) ) || 
        ( *state == MENU_BATTLE_PLAYER_VS_CUSTOM && (!ally_player_game.alive || !enemy_ai_game.alive) ) || 
        ( *state == MENU_BATTLE_PLAYER_VS_PREBUILT && (!ally_player_game.alive || !enemy_ai_game.alive) ) ){
        // game_over = true; what will actually happen

        ally_score += !enemy_ai_game.alive;
        enemy_score += !ally_ai_game.alive || !ally_player_game.alive;

        game_over = (ally_score >= max_score || enemy_score >= max_score) || ((ally_score >= first_to || enemy_score >= first_to) && abs(ally_score - enemy_score) >= win_by);

        if(game_over){
            if(ally_score > enemy_score){
                std::cout << "Ally wins!" << std::endl;
            }
            else{
                std::cout << "Enemy wins!" << std::endl;
            }

            this->ally_score = 0;
            this->enemy_score = 0;
        }

        ally_ai_game.reset();
        enemy_ai_game.reset();
        ally_player_game.reset();
        // ally_ai_game.set_enemy_game(&enemy_ai_game);
        // enemy_ai_game.set_enemy_game(&ally_ai_game);

        if(*state == MENU_BATTLE_CUSTOM_VS_PREBUILT){
            ally_ai_game.set_enemy_game(&enemy_ai_game);
            enemy_ai_game.set_enemy_game(&ally_ai_game);
        } else if (*state == MENU_BATTLE_PLAYER_VS_CUSTOM){
            ally_player_game.set_enemy_game(&enemy_ai_game);
            enemy_ai_game.set_enemy_game(&ally_player_game);
        } else if (*state == MENU_BATTLE_PLAYER_VS_PREBUILT){
            ally_player_game.set_enemy_game(&enemy_ai_game);
            enemy_ai_game.set_enemy_game(&ally_player_game);
        }

        SDL_Delay(2000);
    }

}

