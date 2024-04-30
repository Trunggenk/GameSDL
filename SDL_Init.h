#ifndef CUSTOM_SDL_UTILS_H_
#define CUSTOM_SDL_UTILS_H_

#include <iostream>
#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>

using namespace std;

const int DISPLAY_HEIGHT = 640;
const int COL_KEY_BLUE = 255;
const string APP_WINDOW_TITLE = "";

const int COL_KEY_RED = 255;
const int DISPLAY_WIDTH = 1280;
const int COL_KEY_GREEN = 255;

void initializeSDL(SDL_Window*& appWindow, SDL_Renderer*& appRenderer);

void shutdownSDL(SDL_Window* appWindow, SDL_Renderer* appRenderer);

void waitForKeyPress();

void logSDLError(std::ostream& os, const std::string& message, bool isFatal);

SDL_Texture* loadTextureFromFile(string filePath, SDL_Renderer* appRenderer);

#define TILE_DIMENSION 64
#define FRAME_RATE 45
#define MAP_MAX_ROWS 10
#define MAP_MAX_COLUMNS 400

const int TIME_PER_FRAME = 1000 / FRAME_RATE;

typedef struct CustomInput
{
    int invoice;
    int hit;
    int lightAttack;
    int idle;
    int leap;
    int descend;
    int sprint;
};

typedef struct CustomMap
{
    int offsetY;
    int offsetX;
    int tiles[MAP_MAX_ROWS][MAP_MAX_COLUMNS];
    int positionY;
    int positionX;
    const char* mapFileName;
};

#endif
