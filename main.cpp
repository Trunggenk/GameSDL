#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Monster.h"
#include "Bars_Health.h"
#include "Menu.h"
#include "Time.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event e;
Mix_Music* sound;
TTF_Font* h1font;
TTF_Font* h2font;

bool Init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Game Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        return false;
    }

    if (TTF_Init() == -1) {
        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        return false;
    }

    return true;
}

void Close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(sound);
    TTF_CloseFont(h1font);
    TTF_CloseFont(h2font);

    window = NULL;
    renderer = NULL;
    sound = NULL;
    h1font = NULL;
    h2font = NULL;

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* LoadTexture(string path) {
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (newTexture == NULL) {
        cout << "Unable to load texture! SDL_image Error: " << IMG_GetError() << endl;
    }
    return newTexture;
}

vector<Monster*> CreateMonsterList(int count, int type, double startPos, double endPos) {
    vector<Monster*> monsters;
    Monster* monsterArray = new Monster[20];
    string imgPath = (type == 1) ? "assets/images/monsters/1/m1_run_left.png" : "assets/images/monsters/3/m3_run_left.png";

    for (int i = 0; i < count; i++) {
        Monster* monster = (monsterArray + i);
        monster->set_type(type);
        monster->LoadImg(imgPath, renderer);
        monster->set_clips();
        monster->set_x_pos((startPos + i * ((endPos - startPos) / count)) * TILE_SIZE);
        monster->set_Mid(monster->get_x_pos());
        monster->set_y_pos(2 * TILE_SIZE);
        monster->set_range(monster->get_x_pos());
        monster->set_Mon_HP(type);
        monsters.push_back(monster);
    }
    return monsters;
}

void RefreshMonsters(vector<Monster*>& monsters, currentMap gameMap, Player& gamePlayer) {
    for (auto& monster : monsters) {
        if (monster && monster->Is_Dead()) {
            monster = nullptr;
            continue;
        }
        if (monster && monster->Meet_Sasuke_(gameMap)) {
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
        } else if (monster && !(monster->Meet_Sasuke_(gameMap))) {
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

int main(int argc, char* argv[]) {
    if (!Init()) {
        cout << "Failed to initialize!" << endl;
        return -1;
    }

    sound = Mix_LoadMUS("assets/sounds/gameost.mp3");
    if (sound == NULL) {
        cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << endl;
        return -1;
    }
    Mix_PlayMusic(sound, -1);

    SDL_Texture* startBackground = LoadTexture("assets/images/menu/start.jpg");
    SDL_Texture* gameBackground = LoadTexture("assets/images/game_background.jpg");
    SDL_Texture* winBackground = LoadTexture("assets/images/menu/menu_win.png");
    SDL_Texture* loseBackground = LoadTexture("assets/images/menu/menu_lose.png");

    h1font = TTF_OpenFont("assets/fonts/monogram.ttf", 40);
    h2font = TTF_OpenFont("assets/fonts/monogram.ttf", 80);

    Menu startMenu, endMenu;
    startMenu.set_menu_type(0);
    endMenu.set_menu_type(1);

    bool gameOver = false;
    bool gameIsRunning = false;
    Time gameTimer;

    while (!startMenu.run) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                startMenu.exit = true;
            }
            startMenu.HandleInputMenu(e, renderer);
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, startBackground, NULL, NULL);
        startMenu.SetMenu(h2font, renderer);
        SDL_RenderPresent(renderer);
    }

    if (startMenu.run) {
        gameIsRunning = true;
        endMenu.run = true;
    }

    while (gameIsRunning) {
        if (endMenu.run) {
            MapGame mapgame;
            mapgame.Load_Map();
            mapgame.Load_Tiles(renderer);

            Player gamePlayer;
            gamePlayer.y_pos = 0 * TILE_SIZE;
            gamePlayer.LoadImg("assets/images/player/stand_right.png", renderer);
            gamePlayer.Set_Frame();
            vector<Monster*> monsters = CreateMonsterList(10, 1, 120, 117);
            Bars healthBar
