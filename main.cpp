#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

int main(int argc, char* args[]) {
    // Khởi tạo SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Khởi tạo SDL2_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }

    // Đóng SDL2_image
    IMG_Quit();

    // Đóng SDL2
    SDL_Quit();

    printf("SDL2 and SDL2_image libraries initialized and closed successfully!\n");
    return 0;
}