#include "Bar_Health.h"
#include "Time.h"
#include "Monster.h"
#include "Menu.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event mainevent;
Mix_Music* gmusic;
TTF_Font* mainfont;
TTF_Font* menufont;

using namespace std;

vector<Monster*> Monster_list(int number, int type, double x1, double x2)
{
    vector<Monster*> list_monster;
    Monster* mon = new Monster[20];
    string path;
    if (type == 1) path = "Monster/1/m1_run_left.png";
    else if ( type == 3 ) path = "Monster/3/m3_run_left.png";
    for (int i = 0; i < number; i++)
    {
        Monster* new_monster = (mon + i);
        new_monster->set_type(type);
        if (new_monster != NULL)
        {
            new_monster->LoadImg(path, renderer);
            new_monster->set_clips();
            new_monster->set_x_pos((x1 + i * ((x2 - x1) / number))*TILE_SIZE);
            new_monster->set_Mid(new_monster->get_x_pos());
            new_monster->set_y_pos(2*TILE_SIZE);
            new_monster->set_range(new_monster->get_x_pos());
            new_monster->set_Mon_HP(type);
            list_monster.push_back(new_monster);
        }
    }
    return list_monster;
}

void set_Monster(vector<Monster*>& list_monster, Map mymap, Player& player)
{
    for (int i = 0; i < list_monster.size(); i++)
    {
        Monster* mymon = list_monster[i];
            if (mymon != nullptr && mymon->Meet_Sasuke_(mymap))
            {
                mymon->SetMapXY(mymap.start_x_, mymap.start_y_);
                mymon->MonsterMove(mymap);
                if (player.x_pos < mymon->get_begin() || player.x_pos > mymon->get_end())
                {
                    mymon->MoveAround();
                }
                else
                {
                    mymon->Action(player);
                    mymon->Attack(player);
                    mymon->Is_Attacked(player);
                }
                mymon->Present(renderer);
            }
            else if (mymon != nullptr && !(mymon->Meet_Sasuke_(mymap)))
            {
                if (player.x_pos + SCREEN_WIDTH >= mymon->get_x_pos() && player.x_pos - SCREEN_WIDTH <= mymon->get_x_pos())
                {
                    mymon->SetMapXY(mymap.start_x_, mymap.start_y_);
                    mymon->MonsterMove(mymap);
                    mymon->MoveAround();
                    mymon->Present(renderer);
                }
            }
            if (mymon != nullptr && mymon->Is_Dead())
            {
                list_monster[i] = nullptr;
            }
        }
}

int main(int argc, char* argv[])
{
    bool quit = false;
    bool is_running = false;
    Time maintime;
    initSDL(window, renderer);
    gmusic = Mix_LoadMUS("SadnessAndSorrow.wav");
    if (Mix_PlayingMusic() == 0)
    {
        //Play the music
        Mix_PlayMusic(gmusic, -1);
    }

    mainfont = TTF_OpenFont("font//dlxfont_.ttf", 15);
    menufont = TTF_OpenFont("font//dlxfont_.ttf", 40);

    Menu End_Menu;
    End_Menu.set_menu_type(1);
    End_Menu.set_color();

    //Start_menu
    SDL_Texture* background = loadBackGround("Menu/Menu_Start.png", renderer);
    Menu Start_Menu;
    Start_Menu.set_menu_type(0);
    Start_Menu.set_color();
    while (!Start_Menu.run && !Start_Menu.exit)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL); 
        Start_Menu.SetMenu(menufont, renderer);
        while (SDL_PollEvent(&mainevent) != 0)
        {
            if (mainevent.type == SDL_QUIT)
            {
                Start_Menu.exit = true;
            }
            Start_Menu.HandleInputMenu(mainevent, renderer);
        }
        SDL_RenderPresent(renderer);
    }
    
    if (Start_Menu.run)
    {
       background = loadBackGround("NarutoBackground2.jpeg", renderer);
       End_Menu.run = true;
       is_running = true;
    }
    
    //Menu_Loof
    while (is_running)
    {
        if (End_Menu.run)
        {
            Text time;
            time.SetColor(Text::WHITE_TEXT);
            Text mark;
            mark.SetColor(Text::WHITE_TEXT);

            SDL_RenderCopy(renderer, background, NULL, NULL);

            MapGame mapgame;
            mapgame.Load_Map("map/map.dat");
            mapgame.Load_Tiles(renderer);

            vector<Monster*> monster1_turn_1 = Monster_list(4, 1, 105, 117);
            vector<Monster*> monster1_turn_2 = Monster_list(3, 1, 165, 171);
            vector<Monster*> monster1_turn_3 = Monster_list(2, 1, 243, 245);
            vector<Monster*> monster3_turn_2 = Monster_list(2, 3, 165, 171);
            vector<Monster*> monster3_turn_3 = Monster_list(2, 3, 241, 246);

            Player player;
            player.y_pos = 0 * TILE_SIZE;

            player.LoadImg("Player/sasuke_stand_right_official.png", renderer);
            player.Set_Frame();

            string score;

            Bar_Health HealthBar;
            HealthBar.set_type(0);
            HealthBar.LoadImg("Bar_Health/HealthBar.png", renderer);
            HealthBar.Set_Frame();

            maintime.run_game();
            while (!quit)
            {
                maintime.begin();
                while (SDL_PollEvent(&mainevent) != 0)
                {
                    if (mainevent.type == SDL_QUIT)
                    {
                        quit = true;
                        End_Menu.run = false;
                        is_running = false;
                    }
                    if (player.death || player.check_win())
                    {
                        End_Menu.check_menu_win(player.check_win());
                        quit = true;
                    }
                    player.InputAction(mainevent, renderer);
                }
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);

                Map mymap = mapgame.GetMap();

                player.SetMoveMap(mymap.start_x_, mymap.start_y_);
                player.Move(mymap);
                player.Attacked();
                player.Set_Frame();
                player.Collect_Point(mymap);
                player.Open_Chain(mymap);
                player.Present(renderer);

                player.Is_Attacked_Left = false;
                player.Is_Attackef_Right = false;

                HealthBar.Present(player, renderer);

                mapgame.SetMap(mymap);
                mapgame.Draw_Map(renderer);

                set_Monster(monster1_turn_1, mymap, player);
                set_Monster(monster1_turn_2, mymap, player);
                set_Monster(monster1_turn_3, mymap, player);
                set_Monster(monster3_turn_2, mymap, player);
                set_Monster(monster3_turn_3, mymap, player);

                string game_time = "Time: ";
                Uint32 time_value = maintime.time_played() / 1000;
                string str_value = to_string(time_value);
                game_time += str_value;
                time.SetText(game_time);
                time.LoadFromRenderText(mainfont, renderer);
                time.RenderText(renderer, SCREEN_WIDTH - 2.5 * TILE_SIZE, 15);

                string mark_count = "Mark: ";
                Uint32 eye_collected = player.collected_point;
                string mark_value = to_string(eye_collected);
                mark_count += mark_value;
                mark.SetText(mark_count);
                mark.LoadFromRenderText(mainfont, renderer);
                mark.RenderText(renderer, SCREEN_WIDTH / 2 - TILE_SIZE, 15);

                score = "YOUR SCORE: " + str_value + "s";

                SDL_RenderPresent(renderer);

                int real_time = maintime.get_time();
                if (real_time < time_present_one_frame)
                {
                    SDL_Delay(time_present_one_frame - real_time);
                }
            }

            SDL_RenderClear(renderer);

            if (End_Menu.run)
            {
                if (End_Menu.win)
                {
                    background = loadBackGround("Menu/Menu_Win.png", renderer);
                    End_Menu.get_score(score);
                }
                else background = loadBackGround("Menu/Menu_Lose.png", renderer);
                End_Menu.set_color();
            }

            while (!End_Menu.menu_close && End_Menu.run)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);
                End_Menu.SetMenu(menufont, renderer);
                SDL_RenderPresent(renderer);
                while (SDL_PollEvent(&mainevent) != 0)
                {
                    if (mainevent.type == SDL_QUIT)
                    {
                        End_Menu.run = false;
                        End_Menu.menu_close = true;
                        is_running = false;
                    }
                    End_Menu.HandleInputMenu(mainevent, renderer);
                }
            }
            if (End_Menu.run)
            {
                background = loadBackGround("NarutoBackground2.jpeg", renderer);
                quit = false;
                End_Menu.menu_close = false;
            }
            else is_running = false;
        }
    }

    quitSDL(window, renderer);
    return 0;
}







