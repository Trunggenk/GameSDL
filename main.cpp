#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
using namespace std;
int main(int argc, char* args[]) {
    // Khởi tạo SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Khởi tạo SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    // Đóng SDL_ttf
    TTF_Quit();

    // Đóng SDL2
    SDL_Quit();

    return 0;
}
