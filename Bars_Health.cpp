#include "HealthBar.h"
#include <random>

HealthBar::HealthBar() : width(0), height(0), currentFrame(0), posX(0), posY(0), type(-1), texture(nullptr, SDL_DestroyTexture) {}

HealthBar::~HealthBar() {}

int HealthBar::CalculateFrameCount(int type) {
    return (type == 0) ? 6 : 4;
}

int HealthBar::SelectRandomFrame(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

SDL_Color HealthBar::DetermineColor(int hp) {
    if (hp > 7500) return {0, 255, 0, 255}; // Green for high health
    else if (hp > 5000) return {255, 255, 0, 255}; // Yellow for medium health
    else if (hp > 2500) return {255, 165, 0, 255}; // Orange for low health
    else return {255, 0, 0, 255}; // Red for critical health
}

bool HealthBar::LoadTexture(const std::string& filepath, SDL_Renderer* renderer) {
    SDL_Texture* rawTexture = Object::LoadImg(filepath, renderer); // Assuming Object::LoadImg is defined elsewhere
    if (rawTexture) {
        texture.reset(rawTexture);
        width = rect_.w / CalculateFrameCount(type);
        height = rect_.h;
        return true;
    }
    return false;
}

void HealthBar::SetupFrames() {
    int frameCount = CalculateFrameCount(type);
    for (int i = 0; i < frameCount; ++i) {
        frames[i].x = i * width;
        frames[i].y = 0;
        frames[i].w = width;
        frames[i].h = height;
    }
}

void HealthBar::Render(Player& character, SDL_Renderer* destination) {
    if (type == Health) { // Assuming Health is defined as an enum or constant elsewhere
        LoadTexture("HealthBar.png", destination);
    }

    rect_.x = posX;
    rect_.y = posY;

    currentFrame = SelectRandomFrame(0, CalculateFrameCount(type) - 1);
    SDL_Color color = DetermineColor(character.HP);
    SDL_SetTextureColor
