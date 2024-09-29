#include <iostream>
#include <string>
#include <vector>

#include "../tetris_lib/Game.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class SceneBattle{
    public: 
        AIGame ally_ai_game;
        PlayerGame ally_player_game;
        AIGame enemy_ai_game;
        // PlayerGame* enemy_player_game; prob not needed

        int ally_score;
        int enemy_score;
        int win_by;
        int first_to;
        const int max_score = 9;

        bool game_over;

        SceneBattle(){
            ally_ai_game = AIGame();
            ally_player_game = PlayerGame();
            enemy_ai_game = AIGame();
            ally_score = 0;
            enemy_score = 0;
            win_by = 2;
            first_to = 5;
            game_over = false;
        }
        //SceneBattle(AIGame* ally_ai_game, PlayerGame* ally_ai_game, AIGame* enemy_ai_game);

        void init(int width, int height, SDL_Renderer* renderer, int mode, std::vector<int> gene_seq_0, std::vector<int> gene_seq_1, int ai_pps, int ai_depth, int initial_move_cd, int move_cd, int soft_drop_cd);
        
        const char* back_button_str = "Back (ESC)";
        SDL_Texture* back_button_texture;
        SDL_Rect back_button_text_rect;
        SDL_Rect back_button_rect;

        const char* score_str = " - ";
        SDL_Texture* score_texture;
        SDL_Rect score_rect;
        std::vector<SDL_Texture*> score_digit_textures;
        std::vector<SDL_Rect> score_digit_rects_ally;
        std::vector<SDL_Rect> score_digit_rects_enemy;

        const char* player_name_str = "You";
        const char* cai_name_str = "[INS AI NAME]";
        const char* pai_name_str = "[INS PRESET NAME]";
        SDL_Texture* player_name_texture;
        SDL_Texture* cai_name_texture;
        SDL_Texture* pai_name_texture;
        SDL_Rect player_name_rect_ally;
        SDL_Rect cai_name_rect_ally;
        SDL_Rect cai_name_rect_enemy;
        SDL_Rect pai_name_rect_enemy;

        SDL_Color text_color = {0xee, 0xee, 0xee, 0xff};

        void updateDeltaTime(SDL_Renderer* renderer, SDL_Event* event, int dt, bool* isRunning, int* state);
        void render(SDL_Renderer* renderer, int state);

};