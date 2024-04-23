#pragma once

#include <string>
#include "SDL.h"
#include "Object.h"
#include "Map.h"

struct InputState {
    int stand;
    int run;
    int jump;
    int normalAttack;
    int attacked;
    int specialAttack;
    int fall;
};

class Player {
public:
    Player();
    ~Player();

    bool LoadImage(std::string path, SDL_Renderer* renderer);
    void SetAnimationFrames();
    void Render(SDL_Renderer* des);
    void HandleInput(SDL_Event events, SDL_Renderer* renderer);
    void Move(Map& mymap);
    void CheckCollision(Map& mymap);
    void UpdateMapPosition(Map& mymap);
    void HandleAttack();
    void CollectPoints(Map& mymap);
    bool CheckWinCondition();
    void OpenGate(Map& mymap);

private:
    int currentFrame;
    int posX, posY;
    int velX, velY;
    int characterWidth, characterHeight;
    int direction;
    InputState inputState;
    bool isOnGround;
    int mapX, mapY;
    int groundY;
    bool isDead;
    int attackCount;
    bool isAttackedFromLeft;
    bool isAttackedFromRight;
    int pointsCollected;
    int health;
    int attackedBy;
    int healthDecrease;
    SDL_Rect gif[30];
    SDL_Rect rect_;
    SDL_Texture* objectTexture;
    bool changeHealthBar;
    int frameNumber(InputState input);
};