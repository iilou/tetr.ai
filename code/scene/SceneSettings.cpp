#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <cmath>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_timer.h"

#include "SceneSettings.h"
#include "SCENENAV.h"

SettingOption::SettingOption(const char* n, const char* d, int min, int max, int current, int s){
    name = n;
    description = d;
    min_val = min;
    max_val = max;
    current_val = current;
    old_val = current;
    default_val = current;
    step = s;
}

void SettingOption::set(const char* n, const char* d, int min, int max, int current, int s){
    name = n;
    description = d;
    min_val = min;
    max_val = max;
    current_val = current;
    old_val = current;
    step = s;
}

int SettingOption::init(SDL_Renderer* rend, int off_x, int off_y, int lim_w, int font_size, SDL_Color& color, SDL_Color& hover_color){
    TTF_Font* font = TTF_OpenFont("HunDIN1451.ttf", font_size);

    int name_w, min_w, max_w, val_w, step_w, reset_w;
    int margin_main = 40;
    int margin = 10;

    SDL_Surface* name_surf = TTF_RenderText_Solid(font, name, color);
    SDL_Surface* min_surf = TTF_RenderText_Solid(font, " - ", color);
    SDL_Surface* max_surf = TTF_RenderText_Solid(font, " + ", color);
    SDL_Surface* reset_surf = TTF_RenderText_Solid(font, "RESET", color);

    SDL_Surface* name_surf_hover = TTF_RenderText_Solid(font, name, hover_color);
    SDL_Surface* min_surf_hover = TTF_RenderText_Solid(font, " - ", hover_color);
    SDL_Surface* max_surf_hover = TTF_RenderText_Solid(font, " + ", hover_color);
    SDL_Surface* reset_surf_hover = TTF_RenderText_Solid(font, "RESET", hover_color);

    int height = std::max(std::max(name_surf->h, min_surf->h), max_surf->h);

    name_tex = SDL_CreateTextureFromSurface(rend, name_surf);
    dec_tex = SDL_CreateTextureFromSurface(rend, min_surf);
    inc_tex = SDL_CreateTextureFromSurface(rend, max_surf);
    reset_tex = SDL_CreateTextureFromSurface(rend, reset_surf);

    name_tex_hover = SDL_CreateTextureFromSurface(rend, name_surf_hover);
    dec_tex_hover = SDL_CreateTextureFromSurface(rend, min_surf_hover);
    inc_tex_hover = SDL_CreateTextureFromSurface(rend, max_surf_hover);
    reset_tex_hover = SDL_CreateTextureFromSurface(rend, reset_surf_hover);

    name_w = name_surf->w * 1.7;
    min_w = min_surf->w * 1.7;
    max_w = max_surf->w * 1.7;
    reset_w = reset_surf->w * 1.7;
    val_w = 250;

    assert(lim_w/2 > name_w + margin_main/2 && lim_w/2 > margin_main/2 + min_w + margin + val_w + margin + max_w + margin + reset_w);

    int mid_left = lim_w/2 - margin_main/2;
    int mid_right = lim_w/2 + margin_main/2;

    name_rect = {mid_left - name_w, off_y, name_w, height};
    dec_rect = {mid_right, off_y, min_w, height};
    val_rect = {mid_right + min_w + margin, off_y, val_w, height};
    inc_rect = {mid_right + min_w + margin + val_w + margin, off_y, max_w, height};
    reset_rect = {mid_right + min_w + margin + val_w + margin + max_w + margin, off_y, reset_w, height};

    name_tex_rect = {name_rect.x + (name_rect.w - name_surf->w), name_rect.y + (name_rect.h - name_surf->h) / 2, name_surf->w, name_surf->h};
    dec_tex_rect = {dec_rect.x + (dec_rect.w - min_surf->w) / 2, dec_rect.y + (dec_rect.h - min_surf->h) / 2, min_surf->w, min_surf->h};
    inc_tex_rect = {inc_rect.x + (inc_rect.w - max_surf->w) / 2, inc_rect.y + (inc_rect.h - max_surf->h) / 2, max_surf->w, max_surf->h};
    reset_tex_rect = {reset_rect.x + (reset_rect.w - reset_surf->w) / 2, reset_rect.y + (reset_rect.h - reset_surf->h) / 2, reset_surf->w, reset_surf->h};

    SDL_FreeSurface(name_surf);
    SDL_FreeSurface(min_surf);
    SDL_FreeSurface(max_surf);
    SDL_FreeSurface(reset_surf);

    SDL_FreeSurface(name_surf_hover);
    SDL_FreeSurface(min_surf_hover);
    SDL_FreeSurface(max_surf_hover);
    SDL_FreeSurface(reset_surf_hover);

    TTF_CloseFont(font);

    return height;
}

void SettingOption::render(SDL_Renderer* rend, std::vector<SDL_Texture*>& num_tex, std::vector<SDL_Rect>& num_rect){
    SDL_RenderCopy(rend, hover_all ? name_tex_hover : name_tex, NULL, &name_tex_rect);
    SDL_RenderCopy(rend, hover_dec ? dec_tex_hover : dec_tex, NULL, &dec_tex_rect);
    SDL_RenderCopy(rend, hover_inc ? inc_tex_hover : inc_tex, NULL, &inc_tex_rect);
    SDL_RenderCopy(rend, hover_reset ? reset_tex_hover : reset_tex, NULL, &reset_tex_rect);

    int val = current_val;
    int digits = val == 0 ? 1 : (int)std::log10(val) + 1;
    int culm_width = 0;
    int indent = 5;
    for(int i = digits - 1; i >= 0; i--){
        int d = val / (int)std::pow(10, i) % 10;
        num_rect[d].x = val_rect.x + culm_width + indent;
        num_rect[d].y = val_rect.y;
        SDL_RenderCopy(rend, num_tex[d], NULL, &num_rect[d]);
        culm_width += num_rect[d].w;
    }
}

void SettingOption::mouse_hover(int x, int y){
    hover_all = y >= name_rect.y && y <= name_rect.y + name_rect.h;
    hover_dec = x >= dec_rect.x && x <= dec_rect.x + dec_rect.w && y >= dec_rect.y && y <= dec_rect.y + dec_rect.h;
    hover_inc = x >= inc_rect.x && x <= inc_rect.x + inc_rect.w && y >= inc_rect.y && y <= inc_rect.y + inc_rect.h;
    hover_reset = x >= reset_rect.x && x <= reset_rect.x + reset_rect.w && y >= reset_rect.y && y <= reset_rect.y + reset_rect.h;
}

void SettingOption::mouse_click(int x, int y){
    if(x >= dec_rect.x && x <= dec_rect.x + dec_rect.w && y >= dec_rect.y && y <= dec_rect.y + dec_rect.h){
        dec();
    }
    if(x >= inc_rect.x && x <= inc_rect.x + inc_rect.w && y >= inc_rect.y && y <= inc_rect.y + inc_rect.h){
        inc();
    }
    if(x >= reset_rect.x && x <= reset_rect.x + reset_rect.w && y >= reset_rect.y && y <= reset_rect.y + reset_rect.h){
        reset();
    }
}




SceneSettings::SceneSettings(){
    title_tex = NULL;
    back_tex = NULL;
    apply_tex = NULL;
    reset_tex = NULL;

    title_tex_rect = {0, 0, 0, 0};
    back_tex_rect = {0, 0, 0, 0};
    apply_tex_rect = {0, 0, 0, 0};
    reset_tex_rect = {0, 0, 0, 0};

    num_tex = std::vector<SDL_Texture*>(10);
    num_rect = std::vector<SDL_Rect>(10);

    options = std::vector<SettingOption>();
    /* settings:
        - fps [30--120] 15

        - ai pps [1--5] 1
        - ai depth [0--2] 1

        - init cd [5--1000] 10
        - move cd [5--1000] 10
        - soft drop cd [5--1000] 10

        - ai training group size [0 -- 2] 1 */
}

int SceneSettings::get_option_val(int ind){
    assert(ind >= 0 && ind < options.size());
    return options[ind].current_val;
}

void SceneSettings::pre_init_options(){
    options = std::vector<SettingOption>();
    options.push_back(SettingOption("FPS", "Frames per second",                             30,     120,    240,    15));
    options.push_back(SettingOption("AI PPS", "AI pieces per second",                       1,      7,      1,      1));
    options.push_back(SettingOption("AI Depth", "AI search depth",                          0,      2,      1,      1));
    options.push_back(SettingOption("Move Initial CD", "Initial cooldown",                  5,      1000,   100,    5));
    options.push_back(SettingOption("Move CD", "Move cooldown",                             5,      1000,   10,     5));
    options.push_back(SettingOption("Soft Drop CD", "Soft drop cooldown",                   5,      1000,   10,     5));
    options.push_back(SettingOption("AI Training Group Size", "AI training group size",     0,      2,      2,      1));

}

void SceneSettings::init(SDL_Renderer* rend, int w, int h){
    TTF_Font* font = TTF_OpenFont("HunDIN1451.ttf", 64);

    int title_h = 60;
    int title_margin_top = 40;
    int title_margin_bottom = 40;

    int collection_margin = 15;

    SDL_Surface* title_surf = TTF_RenderText_Solid(font, "SETTINGS", text_color);
    title_tex = SDL_CreateTextureFromSurface(rend, title_surf);
    title_tex_rect = {w / 2 - title_surf->w / 2, title_margin_top + title_h / 2 - title_surf->h / 2, title_surf->w, title_surf->h};

    SDL_FreeSurface(title_surf);

    int off_y = title_h + title_margin_top + title_margin_bottom;
    int off_x = 50;

    for(int i = 0; i < options.size(); i++){
        int height = options[i].init(rend, off_x, off_y, w, 22, text_color, text_hover_color);
        off_y += height + collection_margin;
    }
    
    TTF_CloseFont(font);
    font = TTF_OpenFont("HunDIN1451.ttf", 22);
    for(int i = 0; i < 10; i++){
        SDL_Surface* num_surf = TTF_RenderText_Solid(font, std::to_string(i).c_str(), text_color);
        num_tex[i] = SDL_CreateTextureFromSurface(rend, num_surf);
        num_rect[i] = {0, 0, num_surf->w, num_surf->h};
        SDL_FreeSurface(num_surf);
    }

    back_hover = false;
    apply_hover = false;
    reset_hover = false;

    int back_w = 200;
    int back_h = 40;
    font = TTF_OpenFont("HunDIN1451.ttf", 28);
    SDL_Surface* back_surf = TTF_RenderText_Solid(font, "BACK", text_color);
    back_tex = SDL_CreateTextureFromSurface(rend, back_surf);
    back_tex_rect = {15 + (back_w - back_surf->w) / 2, 15 + (back_h - back_surf->h) / 2, back_surf->w, back_surf->h};
    back_rect = {15, 15, back_w, back_h};
    SDL_FreeSurface(back_surf);

    int apply_w = 200;
    int apply_h = 40;
    int reset_w = 200;
    int reset_h = 40;
    font = TTF_OpenFont("HunDIN1451.ttf", 28);

    SDL_Surface* apply_surf = TTF_RenderText_Solid(font, "APPLY", text_color);
    SDL_Surface* reset_surf = TTF_RenderText_Solid(font, "RESET", text_color);

    int offx = (w - apply_w - reset_w) / 3;
    int offy = h - apply_h - 15;
    apply_tex = SDL_CreateTextureFromSurface(rend, apply_surf);
    reset_tex = SDL_CreateTextureFromSurface(rend, reset_surf);

    apply_tex_rect = {offx + (apply_w - apply_surf->w) / 2, offy + (apply_h - apply_surf->h) / 2, apply_surf->w, apply_surf->h};
    reset_tex_rect = {offx * 2 + apply_w + (reset_w - reset_surf->w) / 2, offy + (reset_h - reset_surf->h) / 2, reset_surf->w, reset_surf->h};

    apply_rect = {offx, offy, apply_w, apply_h};
    reset_rect = {offx * 2 + apply_w, offy, reset_w, reset_h};

    SDL_FreeSurface(apply_surf);
    SDL_FreeSurface(reset_surf);

    TTF_CloseFont(font);
}

void SceneSettings::render(SDL_Renderer* rend, SDL_Event* event, bool* isRunning, int* state){
    while(SDL_PollEvent(event)){
        if(event->type == SDL_QUIT){
            *isRunning = false;
        }

        if(event->type == SDL_KEYDOWN){
            if(event->key.keysym.sym == SDLK_ESCAPE){
                *state = MENU_DEFAULT;
                for(int i = 0; i < options.size(); i++){
                    options[i].cancel();
                }
                return;
            }
        }

        if(event->type == SDL_MOUSEMOTION){
            int x, y;
            SDL_GetMouseState(&x, &y);
            for(int i = 0; i < options.size(); i++){
                options[i].mouse_hover(x, y);
            }

            back_hover = x >= back_rect.x && x <= back_rect.x + back_rect.w && y >= back_rect.y && y <= back_rect.y + back_rect.h;
            apply_hover = x >= apply_rect.x && x <= apply_rect.x + apply_rect.w && y >= apply_rect.y && y <= apply_rect.y + apply_rect.h;
            reset_hover = x >= reset_rect.x && x <= reset_rect.x + reset_rect.w && y >= reset_rect.y && y <= reset_rect.y + reset_rect.h;
        }

        if(event->type == SDL_MOUSEBUTTONDOWN){
            int x, y;
            SDL_GetMouseState(&x, &y);
            for(int i = 0; i < options.size(); i++){
                options[i].mouse_click(x, y);
            }

            if(x >= back_rect.x && x <= back_rect.x + back_rect.w && y >= back_rect.y && y <= back_rect.y + back_rect.h){
                *state = MENU_DEFAULT;
                for(int i = 0; i < options.size(); i++){
                    options[i].cancel();
                }
                return;
            }

            if(x >= apply_tex_rect.x && x <= apply_tex_rect.x + apply_tex_rect.w && y >= apply_tex_rect.y && y <= apply_tex_rect.y + apply_tex_rect.h){
                for(int i = 0; i < options.size(); i++){
                    options[i].apply();
                }
            }

            if(x >= reset_tex_rect.x && x <= reset_tex_rect.x + reset_tex_rect.w && y >= reset_tex_rect.y && y <= reset_tex_rect.y + reset_tex_rect.h){
                for(int i = 0; i < options.size(); i++){
                    options[i].reset();
                }
            }
        }
    }

    SDL_RenderCopy(rend, title_tex, NULL, &title_tex_rect);

    for(int i = 0; i < options.size(); i++){
        options[i].render(rend, num_tex, num_rect);
    }

    SDL_RenderCopy(rend, back_tex, NULL, &back_tex_rect);
    SDL_RenderCopy(rend, apply_tex, NULL, &apply_tex_rect);
    SDL_RenderCopy(rend, reset_tex, NULL, &reset_tex_rect);

    if(back_hover){
        SDL_SetRenderDrawColor(rend, text_color.r, text_color.g, text_color.b, text_color.a);
        SDL_RenderDrawRect(rend, &back_rect);
    }
    if(apply_hover){
        SDL_SetRenderDrawColor(rend, text_color.r, text_color.g, text_color.b, text_color.a);
        SDL_RenderDrawRect(rend, &apply_rect);
    }
    if(reset_hover){
        SDL_SetRenderDrawColor(rend, text_color.r, text_color.g, text_color.b, text_color.a);
        SDL_RenderDrawRect(rend, &reset_rect);
    }
}