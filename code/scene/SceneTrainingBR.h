#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

#include "../tetris_lib/Game.h"
#include "../tetris_lib/Piece.h"
#include "../model/AIModel.h"

#include <vector>

class SceneTrainingBR{
    public:
        
        std::vector<AIGame> AIGAME_L;
        std::vector<int> gmassoc;
        std::vector<std::vector<int>> grid;
        std::vector<int> genes;

        int death_count;
        std::vector<bool> death_status;
        std::vector<int> mother_genetic_sequence;
        std::vector<int> father_genetic_sequence;
        int generation;

        int R,C,COUNT;

        SceneTrainingBR();
        void init(int r, int c, int count, int s_w, int s_h, int depth, SDL_Renderer* rend, std::vector<int> seq = get_prebuilt(), PieceRandomizer pr = PieceRandomizer(100), int gen=0);
        void update(SDL_Renderer* rend, int dt, SDL_Event* event, bool* isRunning, int* state);   

        double t_time;
        std::vector<double> diag;

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

        void render(SDL_Renderer* rend);

        bool paused;
};