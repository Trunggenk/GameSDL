#include "Menu.h"

using namespace std;

Menu::Menu()
{
    type_menu = -1;
    in_start_button = false;
    in_exit_button = false;
    in_playagain_button = false;
    run = false;
    exit = false;
    menu_close = false;
    win = false;
}

Menu::~Menu()
{
    
}

void Menu::set_color()
{
    if (type_menu == StartMenu)
    {
        start_game.SetColor(Text::BLACK_TEXT);
        exit_game.SetColor(Text::BLACK_TEXT);
    }
    if (type_menu == EndMenu)
    {
        play_again.SetColor(Text::BLACK_TEXT);
        exit_game.SetColor(Text::BLACK_TEXT);
        player_score.SetColor(Text::BLACK_TEXT);
    }
}

void Menu::SetMenu(TTF_Font* font, SDL_Renderer* des)
{
    if (type_menu == StartMenu)
    {
        
        start_game.SetText("START GAME");
        start_game.LoadFromRenderText(font, des);
        start_game.RenderText(des, 3*TILE_SIZE, 5*TILE_SIZE);

        
        exit_game.SetText("EXIT GAME");
        exit_game.LoadFromRenderText(font, des);
        exit_game.RenderText(des, 3* TILE_SIZE, 6.5 * TILE_SIZE);
    }
    if (type_menu == EndMenu)
    {
        
        play_again.SetText("PLAY AGAIN");
        play_again.LoadFromRenderText(font, des);
        play_again.RenderText(des, 10 * TILE_SIZE, 5 * TILE_SIZE);

        exit_game.SetText("EXIT GAME");
        exit_game.LoadFromRenderText(font, des);
        exit_game.RenderText(des, 10 * TILE_SIZE, 6.5 * TILE_SIZE);

        if (win)
        {
            player_score.SetText(score);
            player_score.LoadFromRenderText(font, des);
            player_score.RenderText(des, 9 * TILE_SIZE, 3.5 * TILE_SIZE);
        }
    }
}

void Menu::HandleInputMenu(SDL_Event events, SDL_Renderer* renderer)
{
    switch (events.type)
    {
    case SDL_MOUSEMOTION:
    {
        if (type_menu == StartMenu) {
            if (events.motion.x <= 3 * TILE_SIZE + start_game.GetWidth() && events.motion.x >= 3 * TILE_SIZE && events.motion.y >= 5 * TILE_SIZE && events.motion.y <= 5 * TILE_SIZE + start_game.GetHeight())
            {
                in_start_button = true;
                start_game.SetColor(Text::RED_TEXT);
                exit_game.SetColor(Text::BLACK_TEXT);
            }
            else
            {
                in_start_button = false;
                start_game.SetColor(Text::BLACK_TEXT);
            }
            if (events.motion.x <= 3 * TILE_SIZE + exit_game.GetWidth() && events.motion.x >= 3 * TILE_SIZE && events.motion.y >= 6.5 * TILE_SIZE && events.motion.y <= 6.5 * TILE_SIZE + exit_game.GetHeight())
            {
                in_exit_button = true;
                start_game.SetColor(Text::BLACK_TEXT);
                exit_game.SetColor(Text::RED_TEXT);
            }
            else
            {
                in_exit_button = false;
                exit_game.SetColor(Text::BLACK_TEXT);
            }
        }

        if (type_menu == EndMenu) {
            if (events.motion.x <= 10 * TILE_SIZE + play_again.GetWidth() && events.motion.x >= 10 * TILE_SIZE && events.motion.y >= 5 * TILE_SIZE && events.motion.y <= 5 * TILE_SIZE + play_again.GetHeight())
            {
                in_playagain_button = true;
                play_again.SetColor(Text::RED_TEXT);
                exit_game.SetColor(Text::BLACK_TEXT);
            }
            else
            {
                in_playagain_button = false;
                play_again.SetColor(Text::BLACK_TEXT);
            }
            if (events.motion.x <= 10 * TILE_SIZE + exit_game.GetWidth() && events.motion.x >= 10 * TILE_SIZE && events.motion.y >= 6.5 * TILE_SIZE && events.motion.y <= 6.5 * TILE_SIZE + exit_game.GetHeight())
            {
                in_exit_button = true;
                play_again.SetColor(Text::BLACK_TEXT);
                exit_game.SetColor(Text::RED_TEXT);
            }
            else
            {
                in_exit_button = false;
                exit_game.SetColor(Text::BLACK_TEXT);
            }
        }
    }

    break;
    case SDL_MOUSEBUTTONDOWN:
    {
        if (type_menu == StartMenu) {
            if (in_start_button)
            {
                run = true; exit = false;
                menu_close = true;
            }
            if (in_exit_button)
            {
                exit = true; run = false;
                menu_close = true;
            }
        }  

        if (type_menu == EndMenu) {
            if (in_playagain_button)
            {
                run = true; exit = false;
                menu_close = true;
            }
            if (in_exit_button)
            {
                exit = true;
                run = false;
                menu_close = true;
            }
        }
    }
    break;
    }
}

