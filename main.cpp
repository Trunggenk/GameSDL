#include "Bars_Health.h"
#include "Time.h"
#include "Monster.h"
#include "Menu.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;
Mix_Music *sound;
TTF_Font *h1font;
TTF_Font *h2font;
using namespace std;

vector<Monster *> GenerateMonsterList(int monsterCount, int monsterType, double startPos, double endPos) {
    vector<Monster *> monsterList;
    Monster *monsterArray = new Monster[20];
    string imagePath;
    if (monsterType == 1) imagePath = "Monster/1/m1_run_left.png";
    else if (monsterType == 3) imagePath = "Monster/3/m3_run_left.png";
    for (int i = 0; i < monsterCount; i++) {
        Monster *newMonster = (monsterArray + i);
        newMonster->set_type(monsterType);
        if (newMonster != NULL) {
            newMonster->LoadImg(imagePath, renderer);
            newMonster->set_clips();
            newMonster->set_x_pos((startPos + i * ((endPos - startPos) / monsterCount)) * TILE_SIZE);
            newMonster->set_Mid(newMonster->get_x_pos());
            newMonster->set_y_pos(2 * TILE_SIZE);
            newMonster->set_range(newMonster->get_x_pos());
            newMonster->set_Mon_HP(monsterType);
            monsterList.push_back(newMonster);
        }
    }
    return monsterList;
}
void UpdateMonsters(vector<Monster *> &monsterList, currentMap gameMap, Player &gamePlayer) {
    for (int index = 0; index < monsterList.size(); index++) {
        Monster *currentMonster = monsterList[index];
        if (currentMonster != nullptr && currentMonster->Is_Dead()) {
            monsterList[index] = nullptr;
            continue;
        }
        if (currentMonster != nullptr && currentMonster->Meet_Sasuke_(gameMap)) {
            currentMonster->SetMapXY(gameMap.start_x_, gameMap.start_y_);
            currentMonster->MonsterMove(gameMap);
            if (gamePlayer.x_pos < currentMonster->get_begin() || gamePlayer.x_pos > currentMonster->get_end()) {
                currentMonster->MoveAround();
            } else {
                currentMonster->Action(gamePlayer);
                currentMonster->Attack(gamePlayer);
                currentMonster->Is_Attacked(gamePlayer);
            }
            currentMonster->Present(renderer);
        } else if (currentMonster != nullptr && !(currentMonster->Meet_Sasuke_(gameMap))) {
            if (gamePlayer.x_pos + SCREEN_WIDTH >= currentMonster->get_x_pos() &&
                gamePlayer.x_pos - SCREEN_WIDTH <= currentMonster->get_x_pos()) {
                currentMonster->SetMapXY(gameMap.start_x_, gameMap.start_y_);
                currentMonster->MonsterMove(gameMap);
                currentMonster->MoveAround();
                currentMonster->Present(renderer);
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

    Menu StartGame;
    StartGame.set_menu_type(0);
    StartGame.set_color();

    Menu endMenu;
    endMenu.set_menu_type(1);
    endMenu.set_color();

    h1font = TTF_OpenFont("font/monogram.ttf", 60);
    h2font = TTF_OpenFont("font/monogram.ttf", 60);

    while (!StartGame.run) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                StartGame.exit = true;
            }
            StartGame.HandleInputMenu(e, renderer);
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, gameBackground, NULL, NULL);
        StartGame.SetMenu(h2font, renderer);
        SDL_RenderPresent(renderer);
    }

    if (StartGame.run) {
        gameIsRunning = true;
        endMenu.run = true;
        gameBackground = loadBackGround("images.jpg", renderer);
    }

    //game bat dau render tai day
    while (gameIsRunning) {
        if (endMenu.run) {
            MapGame mapgame;
            mapgame.Load_Map();
            mapgame.Load_Tiles(renderer);

            Player gamePlayer;
            gamePlayer.y_pos = 0 * TILE_SIZE;
            gamePlayer.LoadImg("Player/stand_right.png", renderer);
            gamePlayer.Set_Frame();

            vector<Monster *> monsterList1 = GenerateMonsterList(10, 1, 120, 117);

            Bars HealthBar;
            HealthBar.set_type(0);
            HealthBar.Loadimage("Bars/HealthBar.png", renderer);
            HealthBar.S_Frame();

            string playerScore;

            Text time;
            Text scoreDisplay;
            time.SetColor(Text::WHITE_TEXT);
            scoreDisplay.SetColor(Text::WHITE_TEXT);

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

                HealthBar.Display(gamePlayer, renderer);

                mapgame.SetMap(gameMap);
                mapgame.Draw_Map(renderer);

                UpdateMonsters(monsterList1, gameMap, gamePlayer);

                string gameTimeText = "Time: ";
                Uint32 elapsedTime = gameTimer.time_played() / 1000;
                string elapsedTimeStr = to_string(elapsedTime);
                gameTimeText += elapsedTimeStr;
                time.SetText(gameTimeText);
                time.LoadFromRenderText(h1font, renderer);
                time.RenderText(renderer, SCREEN_WIDTH - 2.5 * TILE_SIZE, 15);

                string scoreText = "Score: ";
                Uint32 collectedPoints = gamePlayer.collected_point;
                string collectedPointsStr = to_string(collectedPoints);
                scoreText += collectedPointsStr;
                scoreDisplay.SetText(scoreText);
                scoreDisplay.LoadFromRenderText(h1font, renderer);
                scoreDisplay.RenderText(renderer, SCREEN_WIDTH / 2 - TILE_SIZE, 15);

                playerScore = "YOUR SCORE: " + elapsedTimeStr + "s";

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
                } else
                    gameBackground = loadBackGround("Menu/Menu_Lose.png", renderer);
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
            } else gameIsRunning = false;
        }
    }

    quitSDL(window, renderer);
    return 0;
}







