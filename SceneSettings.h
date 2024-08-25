#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <cassert>
#include <cmath>

struct SettingOption{
    const char* name;
    const char* description;
    int min_val;
    int max_val;
    int current_val;
    int old_val;
    int default_val;
    int step;

    bool hover_all;
    bool hover_dec;
    bool hover_inc;
    bool hover_reset;

    SDL_Rect name_rect;
    SDL_Rect dec_rect;
    SDL_Rect val_rect;
    SDL_Rect inc_rect;
    SDL_Rect reset_rect;

    SDL_Texture* name_tex;
    SDL_Texture* name_tex_hover;
    SDL_Rect name_tex_rect;
    SDL_Texture* dec_tex;
    SDL_Texture* dec_tex_hover;
    SDL_Rect dec_tex_rect;
    SDL_Texture* inc_tex;
    SDL_Texture* inc_tex_hover;
    SDL_Rect inc_tex_rect;
    SDL_Texture* reset_tex;
    SDL_Texture* reset_tex_hover;
    SDL_Rect reset_tex_rect;
    int max_dig_w;

    SettingOption(const char* n, const char* d, int min, int max, int current, int s);
    void set(const char* n, const char* d, int min, int max, int current, int s);

    int init(SDL_Renderer* rend, int off_x, int off_y, int lim_w, int font_size, SDL_Color& color, SDL_Color& hover_color);
    void render(SDL_Renderer* rend, std::vector<SDL_Texture*>& num_tex, std::vector<SDL_Rect>& num_rect);

    void mouse_hover(int x, int y);
    void mouse_click(int x, int y);

    void inc(){
        current_val = std::min(current_val + step, max_val);
    }

    void dec(){
        current_val = std::max(current_val - step, min_val);
    }

    void cancel(){
        current_val = old_val;
    }

    void reset(){
        current_val = default_val;
    }

    void apply(){
        old_val = current_val;
    }

    void set(int val){
        current_val = val;
    }

};

class SceneSettings{
    public:
        SDL_Texture *title_tex;
        SDL_Rect title_tex_rect;

        std::vector<SettingOption> options;

        SDL_Texture *back_tex;
        SDL_Rect back_tex_rect;
        SDL_Rect back_rect;
        bool back_hover;

        SDL_Texture *apply_tex;
        SDL_Rect apply_rect;
        SDL_Rect apply_tex_rect;
        bool apply_hover;

        SDL_Texture *reset_tex;
        SDL_Rect reset_rect;
        SDL_Rect reset_tex_rect;
        bool reset_hover;

        SDL_Color text_color = {0xe1, 0xe1, 0xe1, 0xff};
        SDL_Color text_sub_color = {0x7f, 0x7f, 0x7f, 0xff};
        SDL_Color text_hover_color = {0xe1, 0x7f, 0x7f, 0xff};

        std::vector<SDL_Texture*> num_tex;
        std::vector<SDL_Rect> num_rect;

        SceneSettings();

        void pre_init_options();

        void init(SDL_Renderer* rend, int w, int h);

        void render(SDL_Renderer* rend, SDL_Event* event, bool* isRunning, int* state);

        int get_option_val(int i);
};