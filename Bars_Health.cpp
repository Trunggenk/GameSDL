#include "Bars_Health.h"

int getFrameNumber(int type_) {
    if (type_ == 0) return 8;
}

Bars::Bars() {
    barWidth = 0;
    barHeight = 0;
    SDL_Rect gif[30];
    currentFrame = 0;
    mapX = 0;
    mapY = 0;
    type = -1;
}

Bars::~Bars() {

}

bool Bars::Loadimage(string path, SDL_Renderer *renderer) {
    bool loadedimg = Object::LoadImg(path, renderer);
    if (loadedimg == true) {
        barWidth = rect_.w / getFrameNumber(type);
        barHeight = rect_.h;
    }
    return loadedimg;
}

void Bars::S_Frame() {
    int frame_number_ = getFrameNumber(type);
    for (int i = 0; i < frame_number_; i += 1) {
        gif[i].x = i * barWidth;
        gif[i].y = 0;
        gif[i].w = barWidth;
        gif[i].h = barHeight;
    }
}

void Bars::Display(Player sake, SDL_Renderer *des) {
    if (type == Health) {
        Loadimage("Bars/HealthBar.png", des);
    }

    rect_.x = mapX;
    rect_.y = mapY;
    if (sake.HP >= 8750) currentFrame = 0;
    else if (sake.HP >= 7000) currentFrame = 1;
    else if (sake.HP >= 1000) currentFrame = 6;
    else if (sake.HP >= 6000) currentFrame = 2;
    else if (sake.HP >= 2000) currentFrame = 5;
    else currentFrame = 7;
    SDL_Rect *current_clip = &gif[currentFrame];
    SDL_Rect renderQuad = {rect_.x, rect_.y, barWidth, barHeight};
    SDL_RenderCopy(des, myobject, current_clip, &renderQuad);
}

