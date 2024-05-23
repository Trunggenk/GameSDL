#include "Monster.h"
#include "Bars_Health.h"
#include "Menu.h"
#include "Time.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;
Mix_Music *sound;
TTF_Font *h1font;
TTF_Font *h2font;
using namespace std;

vector<Monster *> CreateMonsterList(int monsterCount, int monsterType, double startPos, double endPos) {
    vector<Monster *> monsters;
    Monster *monsterArray = new Monster[20];
    string imgPath;
    if (monsterType == 1) imgPath = "Monster/1/m1_run_left.png";
    else if (monsterType == 3) imgPath = "Monster/3/m3_run_left.png";
    for (int i = 0; i < monsterCount; i++) {
        Monster *monster = (monsterArray + i);
        monster->set_type(monsterType);
        if (monster != NULL) {
            monster->LoadImg(imgPath, renderer);
            monster->set_clips();
            monster->set_x_pos((startPos + i * ((endPos - startPos) / monsterCount)) * TILE_SIZE);
            monster->set_Mid(monster->get_x_pos());
            monster->set_y_pos(2 * TILE_SIZE);
            monster->set_range(monster->get_x_pos());
            monster->set_Mon_HP(monsterType);
            monsters.push_back(monster);
        }
    }
    return monsters;
}

void RefreshMonsters(vector<Monster *> &monsters, currentMap gameMap, Player &gamePlayer) {
    for (int i = 0; i < monsters.size(); i++) {
        Monster *monster = monsters[i];
        if (monster != nullptr && monster->Is_Dead()) {
            monsters[i] = nullptr;
            continue;
        }
        if (monster != nullptr && monster->Meet_Sasuke_(gameMap)) {
            monster->SetMapXY(gameMap.start_x_, gameMap.start_y_);
            monster->MonsterMove(gameMap);
            if (gamePlayer.x_pos < monster->get_begin() || gamePlayer.x_pos > monster->get_end()) {
                monster->MoveAround();
            } else {
                monster->Action(gamePlayer);
                monster->Attack(gamePlayer);
                monster->Is_Attacked(gamePlayer);
            }
            monster->Present(renderer);
        } else if (monster != nullptr && !(monster->Meet_Sasuke_(gameMap))) {
            if (gamePlayer.x_pos + SCREEN_WIDTH >= monster->get_x_pos() &&
                gamePlayer.x_pos - SCREEN_WIDTH <= monster->get_x_pos()) {
                monster->SetMapXY(gameMap.start_x_, gameMap.start_y_);
                monster->MonsterMove(gameMap);
                monster->MoveAround();
                monster->Present(renderer);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    initSDL(window, renderer);
    bool gameOver = false;
    bool gameIsRunning = false;
    Time gameTimer;
    sound = Mix_LoadMUS("gameost.mp3");
    Mix_PlayMusic(sound, -1);
    SDL_Texture *gameBackground = loadBackGround("Menu/Start.jpg", renderer);

    Menu startMenu;
    startMenu.set_menu_type(0);
    startMenu.set_color();

    Menu endMenu;
    endMenu.set_menu_type(1);
    endMenu.set_color();

    h1font = TTF_OpenFont("font/monogram.ttf", 40);
    h2font = TTF_OpenFont("font/monogram.ttf", 80);

    while (!startMenu.run) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                startMenu.exit = true;
            }
            startMenu.HandleInputMenu(e, renderer);
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, gameBackground, NULL, NULL);
        startMenu.SetMenu(h2font, renderer);
        SDL_RenderPresent(renderer);
    }

    if (startMenu.run) {
        gameIsRunning = true;
        endMenu.run = true;
        gameBackground = loadBackGround("images.jpg", renderer);
    }

    while (gameIsRunning) {
        if (endMenu.run) {
            MapGame mapgame;
            mapgame.Load_Map();
            mapgame.Load_Tiles(renderer);

            Player gamePlayer;
            gamePlayer.y_pos = 0 * TILE_SIZE;
            gamePlayer.LoadImg("Player/stand_right.png", renderer);
            gamePlayer.Set_Frame();
            vector<Monster *> monster = CreateMonsterList(10, 1, 120, 117);
            Bars healthBar;
            healthBar.set_type(0);
            healthBar.Loadimage("Bars/HealthBar.png", renderer);
            healthBar.S_Frame();
            string playerScore;
            Text timeText;
            Text scoreText;
            timeText.SetColor(Text::WHITE_TEXT);
            scoreText.SetColor(Text::WHITE_TEXT);
            SDL_RenderCopy(renderer, gameBackground, NULL, NULL);
            gameTimer.run_game();
            while (!gameOver) {
                gameTimer.begin();
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        gameOver = true;
                        endMenu.run = false;
                        gameIsRunning = false;
                    }
                    if (gamePlayer.death || gamePlayer.check_win()) {
                        endMenu.check_menu_win(gamePlayer.check_win());
                        gameOver = true;
                    }
                    gamePlayer.InputAction(e, renderer);
                }
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, gameBackground, NULL, NULL);
                currentMap gameMap = mapgame.GetMap();
                gamePlayer.SetMoveMap(gameMap.start_x_, gameMap.start_y_);
                gamePlayer.Move(gameMap);
                gamePlayer.Attacked();
                gamePlayer.Set_Frame();
                gamePlayer.Collect_Point(gameMap);
                gamePlayer.Open_Chain(gameMap);
                gamePlayer.Present(renderer);
                gamePlayer.Is_Attacked_Left = false;
                gamePlayer.Is_Attackef_Right = false;
                healthBar.Display(gamePlayer, renderer);
                mapgame.SetMap(gameMap);
                mapgame.Draw_Map(renderer);
                RefreshMonsters(monsters, gameMap, gamePlayer);
                string gameTimeStr = "Time: ";
                Uint32 elapsedTime = gameTimer.time_played() / 1000;
                string elapsedTimeString = to_string(elapsedTime);
                gameTimeStr += elapsedTimeString;
                timeText.SetText(gameTimeStr);
                timeText.LoadFromRenderText(h1font, renderer);
                timeText.RenderText(renderer, SCREEN_WIDTH - 2.5 * TILE_SIZE, 15);

                string scoreStr = "Score: ";
                Uint32 collectedPoints = gamePlayer.collected_point;
                string collectedPointsString = to_string(collectedPoints);
                scoreStr += collectedPointsString;
                scoreText.SetText(scoreStr);
                scoreText.LoadFromRenderText(h1font, renderer);
                scoreText.RenderText(renderer, SCREEN_WIDTH / 2 - TILE_SIZE, 15);

                playerScore = "YOUR SCORE: " + elapsedTimeString + "s";

                SDL_RenderPresent(renderer);

                int frameTime = gameTimer.get_time();
                if (frameTime < time_present_one_frame) {
                    SDL_Delay(time_present_one_frame - frameTime);
                }
            }

            SDL_RenderClear(renderer);

            if (endMenu.run) {
                if (endMenu.win) {
                    gameBackground = loadBackGround("Menu/Menu_Win.png", renderer);
                    endMenu.get_score(playerScore);
                } else {
                    gameBackground = loadBackGround("Menu/Menu_Lose.png", renderer);
                }
                endMenu.set_color();
            }

            while (!endMenu.menu_close && endMenu.run) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, gameBackground, NULL, NULL);
                endMenu.SetMenu(h2font, renderer);
                SDL_RenderPresent(renderer);
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        endMenu.run = false;
                        endMenu.menu_close = true;
                        gameIsRunning = false;
                    }
                    endMenu.HandleInputMenu(e, renderer);
                }
            }
            if (endMenu.run) {
                gameBackground = loadBackGround("images.jpg", renderer);
                gameOver = false;
                endMenu.menu_close = false;
            } else {
                gameIsRunning = false;
            }
        }
    }

    return 0;
}
