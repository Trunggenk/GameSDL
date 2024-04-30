#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SDL.h>
#include <string>
#include <memory>
#include "Player.h"  // Assuming Player class is defined elsewhere

class HealthBar {
public:
    HealthBar();
    ~HealthBar();
    bool LoadTexture(const std::string& filepath, SDL_Renderer* renderer);
    void SetupFrames();
    void Render(Player& character, SDL_Renderer* destination);

private:
    int width, height, currentFrame;
    SDL_Rect frames[10];
    int posX, posY, type;
    SDL_Rect rect_;
    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> texture;

    int CalculateFrameCount(int type);
    int SelectRandomFrame(int min, int max);
    SDL_Color DetermineColor(int hp);
};

#endif // HEALTHBAR_H
