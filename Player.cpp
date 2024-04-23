#include "Player.h"
#include "SDL_Init.h"
int Player::frameNumber(InputState input) {
    if ((input.jump == 1 || input.fall == 1) && input.normalAttack == 1) return 8;
    if (input.jump == 1 || input.fall == 1) return 6;
    if (input.attacked == 1) return 1;
    if (input.specialAttack == 1) return 19;
    if (input.normalAttack == 1) return 13;
    if (input.stand == 1 || input.run == 1) return 6;
    return 0;
}

Player::Player()
        : currentFrame(0), posX(0), posY(0), velX(0), velY(0), characterWidth(0), characterHeight(0),
          direction(-1), isOnGround(false), mapX(0), mapY(0), groundY(0), isDead(false), attackCount(0),
          isAttackedFromLeft(false), isAttackedFromRight(false), pointsCollected(0), health(10000),
          attackedBy(0), healthDecrease(0), changeHealthBar(false) {
    inputState = {1, 0, 0, 0, 0, 0, 0};
}

Player::~Player() {}

bool Player::LoadImage(std::string path, SDL_Renderer* renderer) {
    bool loaded = Object::LoadImage(path, renderer);
    if (loaded) {
        characterWidth = rect_.w / frameNumber(inputState);
        characterHeight = rect_.h;
    }
    return loaded;
}

void Player::SetAnimationFrames() {
    int frameNum = frameNumber(inputState);
    for (int i = 0; i < frameNum; ++i) {
        gif[i].x = i * characterWidth;
        gif[i].y = 0;
        gif[i].w = characterWidth;
        gif[i].h = characterHeight;
    }
}

void Player::Render(SDL_Renderer* des) {
    std::string imgPath;
    if (direction == Left) {
        if ((inputState.jump == 1 || inputState.fall == 1) && inputState.normalAttack == 1)
            imgPath = "Player/jump_normal_attack_left.png";
        else if (inputState.jump == 1) imgPath = "Player/player_jump_up_left.png";
        else if (inputState.fall == 1) imgPath = "Player/player_jump_down_left.png";
        else if (inputState.attacked == 1) imgPath = "Player/Is_Attacked_Left.png";
        else if (inputState.specialAttack == 1) imgPath = "Player/player_special_attack_left.png";
        else if (inputState.normalAttack == 1) imgPath = "Player/player_attack_left.png";
        else if (inputState.stand == 1) {
            imgPath = "Player/player_stand_left_official.png";
            SDL_Delay(50);
        } else if (inputState.run == 1) imgPath = "Player/player_run_left_official.png";
    } else {
        if ((inputState.jump == 1 || inputState.fall == 1) && inputState.normalAttack == 1)
            imgPath = "Player/jump_normal_attack_right.png";
        else if (inputState.jump == 1) imgPath = "Player/player_jump_up_right.png";
        else if (inputState.fall == 1) imgPath = "Player/player_jump_down_right.png";
        else if (inputState.attacked == 1) imgPath = "Player/Is_Attacked_Right.png";
        else if (inputState.specialAttack == 1) imgPath = "Player/player_special_attack_right.png";
        else if (inputState.normalAttack == 1) imgPath = "Player/player_attack_right.png";
        else if (inputState.stand == 1) {
            imgPath = "Player/stand_right.png";
            SDL_Delay(50);
        } else if (inputState.run == 1) imgPath = "Player/player_run_right_official.png";
    }
    LoadImage(imgPath, des);

    currentFrame++;
    if (inputState.specialAttack == 1) SDL_Delay(50);
    if (currentFrame >= frameNumber(inputState) && inputState.normalAttack == 1 && attackCount <= 5) {
        attackCount++;
        currentFrame = frameNumber(inputState) - 1;
    } else if (currentFrame >= (frameNumber(inputState) - 1) && inputState.specialAttack == 1) {
        currentFrame = 0;
        inputState.specialAttack = 0;
    } else if (currentFrame >= frameNumber(inputState)) currentFrame = 0;

    if (attackCount == 6) {
        attackCount = 0;
        currentFrame = 0;
    }

    if (inputState.specialAttack == 1 && direction == Left) {
        rect_.x = posX - mapX - characterWidth + TILE_SIZE;
        rect_.y = posY - mapY;
    } else {
        rect_.x = posX - mapX;
        rect_.y = posY - mapY;
    }

    SDL_Rect* currentClip = &gif[currentFrame];
    SDL_Rect renderQuad = {rect_.x, rect_.y, characterWidth, characterHeight};
    SDL_RenderCopy(des, objectTexture, currentClip, &renderQuad);
}

void Player::HandleInput(SDL_Event events, SDL_Renderer* renderer) {
    if (events.type == SDL_KEYDOWN) {
        switch (events.key.keysym.sym) {
            case SDLK_k:
                if (inputState.specialAttack == 0) currentFrame = 0;
                if (isOnGround) inputState.specialAttack = 1;
                break;
            case SDLK_j:
                inputState.normalAttack = 1;
                inputState.specialAttack = 0;
                break;
            case SDLK_w:
                if (isOnGround) {
                    inputState.jump = 1;
                    groundY = posY;
                    isOnGround = false;
                }
                inputState.specialAttack = 0;
                break;
            case SDLK_d:
                if (!(isOnGround == false && direction == Left && velX != 0)) {
                    direction = Right;
                    inputState.run = 1;
                    inputState.stand = 0;
                    inputState.specialAttack = 0;
                }
                break;
            case SDLK_a:
                if (!(isOnGround == false && direction == Right && velX != 0)) {
                    direction = Left;
                    inputState.run = 1;
                    inputState.stand = 0;
                    inputState.specialAttack = 0;
                }
                break;
            default:
                break;
        }
    } else if (events.type == SDL_KEYUP) {
        switch (events.key.keysym.sym) {
            case SDLK_w:
                inputState.fall = 1;
                inputState.jump = 0;
                break;
            case SDLK_j:
                inputState.normalAttack = 0;
                break;
            default:
                inputState.run = 0;
                inputState.stand = 1;
                break;
        }
    }
}

void Player::Move(Map& mymap) {
    velX = 0;
    velY += GRAVITY;

    if (velY >= MAX_FALL_SPEED) velY = MAX_FALL_SPEED;
    if (inputState.run == 1) {
        if (direction == Right) velX += SPEED;
        else if (direction == Left) velX -= SPEED;
    }
    if (inputState.jump == 1) {
        velY -= JUMP_SPEED;
    } else if (inputState.fall == 1) {
        velY += JUMP_SPEED;
    }
    if (inputState.normalAttack == 1 && inputState.run == 1) {
        if (