#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

#include "Game.h"
#include "Piece.h"
#include "Collision.h"
#include "constants.h"
#include "AIHeuristics.h"
#include "AIModel.h"

class SceneTrainingSV{
    public:
        
        // std::vector<AIGame> AIGAME_L;
        // std::vector<int> gmassoc;
        // std::vector<std::vector<int>> grid;
        // std::vector<int> genes;

        // int death_count;
        // std::vector<bool> death_status;
        // std::vector<int> mother_genetic_sequence;
        // std::vector<int> father_genetic_sequence;

        // int R,C,COUNT;

        // SceneTrainingBR();
        // void init(int r, int c, int count, int s_w, int s_h, SDL_Renderer* rend, std::vector<int> seq = get_prebuilt(), PieceRandomizer pr = PieceRandomizer(100));
        // void update(SDL_Renderer* rend, int dt);

        // double t_time;
        // std::vector<double> diag;

        std::vector<AIGame> AIGAME_L;
        double line_timer, line_timer_dur, line_timer_factor, line_timer_initial;
        int score_threshold, end_timer, end_timer_dur;

        std::vector<std::vector<int>> grid;
        std::vector<int> genes;

        int death_count;
        std::vector<bool> death_status;
        std::vector<int> mother_genetic_sequence;
        std::vector<int> father_genetic_sequence;

        int R,C,COUNT;

        int generation;

        SceneTrainingSV();
        void init(int r, int c, int count, int s_w, int s_h, int depth, SDL_Renderer* rend, std::vector<int> seq = get_prebuilt(), PieceRandomizer pr = PieceRandomizer(100), int gen=0);
        void end_game();
        void update(SDL_Renderer* rend, int dt, SDL_Event* event, bool* isRunning, int* state);

        void render(SDL_Renderer* rend);

        
        SDL_Texture* menu_tex;
        SDL_Texture* generation_count_tex;
        std::vector<SDL_Texture*> dig_tex;
        SDL_Texture* confirm_tex;

        SDL_Rect menu_rect;
        SDL_Rect menu_bg;
        SDL_Rect generation_count_rect;
        std::vector<SDL_Rect> dig_rect;
        SDL_Rect generation_count_bg;
        SDL_Rect confirm_rect;
        SDL_Rect confirm_bg;
        int dig_max_width;

        //color
        SDL_Color menu_color;
        SDL_Color generation_count_color;
        SDL_Color dig_color;
        SDL_Color confirm_color;

        SDL_Color menu_bg_color;
        SDL_Color generation_count_bg_color;
        SDL_Color confirm_bg_color;

        bool paused;
};