#include <iostream>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class SceneMenu{

    public:
        const char* title_str = "Tetris AI";
        SDL_Texture* title_text;
        SDL_Rect title_text_rect;
        SDL_Rect title_rect;

        std::vector<const char*> menu_items = {
            "Battle",
            "Train",
            "Settings",
            "Quit"
        };
        std::vector<SDL_Texture*> menu_item_texts;
        std::vector<SDL_Texture*> menu_item_texts_highlighted;
        std::vector<SDL_Rect> menu_item_text_rects;
        std::vector<bool> menu_item_highlighted;

        const char* menu_exit_str = "Back (ESC)";
        SDL_Texture* battle_exit_text;
        SDL_Rect battle_exit_text_rect;
        SDL_Rect battle_exit_rect;


        // ------------------------------ BATTLE MENU ------------------------------

        std::vector<const char*> battle_menu_items = {
            "-> Player vs Custom AI",
            "-> Player vs Prebuilt AI",
            "-> Custom AI vs Prebuilt AI"
        };
        std::vector<SDL_Texture*> battle_menu_item_texts;
        std::vector<SDL_Rect> battle_menu_item_text_rects;
        std::vector<SDL_Rect> battle_menu_item_rects;
        std::vector<bool> battle_menu_item_highlighted;

        // ------------------------------ TRAINING MENU ------------------------------

        std::vector<const char*> training_menu_items = {
            "-> Battle Royale",
            "-> Sprint Challenge",
            "-> Survival Challenge"
        };
        std::vector<SDL_Texture*> training_menu_item_texts;
        std::vector<SDL_Rect> training_menu_item_text_rects;
        std::vector<SDL_Rect> training_menu_item_rects;
        std::vector<bool> training_menu_item_highlighted;

        SDL_Color title_color = {0xd9, 0xd9, 0xd9, 0xff};
        SDL_Color bg_color = {0x10, 0x10, 0x10, 0xff};
        SDL_Color menu_color = {0x80, 0x80, 0x80, 0xff};
        SDL_Color menu_highlight_color = {0xd0, 0x79, 0x79, 0xff};
        SDL_Color battle_item_text_color = {0xee, 0xee, 0xee, 0xff};

        SceneMenu();

        void init(int areaWidth, int areaHeight, SDL_Renderer* renderer);
        void render(SDL_Renderer* renderer, SDL_Event* event, bool* isRunning, int* state);
};