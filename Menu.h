#ifndef MENU_H_
#define MENU_H_

#include "Text.h"

class Menu {
public:
    Menu();

    ~Menu();

    enum type_menu {
        StartMenu = 0,
        EndMenu = 1,
    };

    void set_menu_type(int type_) { type_menu = type_; }

    void HandleInputMenu(SDL_Event events, SDL_Renderer *renderer);

    void SetMenu(TTF_Font *font, SDL_Renderer *des);

    void check_menu_win(bool sakewin) { win = sakewin; }

    void get_score(string score_) { score = score_; }

    void set_color();

    int type_menu;
    Text start_game;
    Text exit_game;
    Text resume;
    Text play_again;
    Text player_score;
    bool in_start_button;
    bool in_exit_button;
    bool in_playagain_button;
    bool run;
    bool exit;
    bool menu_close;
    bool win;
    string score;
};

#endif
