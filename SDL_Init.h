#ifndef MODIFIED_SDL_FUNCTION_H_
#define MODIFIED_SDL_FUNCTION_H_

#include <iostream>
#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>

using namespace std;

const int SCREEN_HEIGHT_MODIFIED = 640;
const int R_MOD_COLORKEY_B = 255;
const string WINDOW_TITLE_MODIFIED = "";

const int R_MOD_COLORKEY_R = 255;
const int SCREEN_WIDTH_MODIFIED = 1280;
const int R_MOD_COLORKEY_G = 255;

void initModifiedSDL(SDL_Window*& window, SDL_Renderer*& renderer);

void quitModifiedSDL(SDL_Window* window, SDL_Renderer* renderer);

void waitUntilModifiedKeyPressed();

void logModifiedSDLError(std::ostream& os,
                         const std::string& msg, bool fatal);

SDL_Texture* loadModifiedBackGround(string path, SDL_Renderer* renderer);

#define TILE_SIZE_MODIFIED 64
#define FPS_MODIFIED 45
#define MAX_Y_MODIFIED 10
#define MAX_X_MODIFIED 400

const int time_present_one_frame_modified = 1000 / FPS_MODIFIED;

typedef struct ModifiedInput
{
    int HoaDon;
    int attacked;
    int normal_attack;
    int stand;
    int jump;
    int fall;
    int run;
};

typedef struct ModifiedMap
{
    int start_y_;
    int start_x_;
    int tile[MAX_Y_MODIFIED][MAX_X_MODIFIED];
    int vitri_y_;
    int vitri_x_;
    const char* mapname;
};

#endif
