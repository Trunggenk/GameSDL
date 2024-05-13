#include "Object.h"
#include "SDLFunc.h"

using namespace std;

Object::Object() {
    myobject = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}

Object::~Object() {
    Free();
}

bool Object::LoadImg(string path, SDL_Renderer *renderer) {
    SDL_Texture *new_Texture = NULL;
    SDL_Surface *loadedsurface = IMG_Load(path.c_str());
    if (loadedsurface != NULL) {
        SDL_SetColorKey(loadedsurface, SDL_TRUE, SDL_MapRGB(loadedsurface->format, Colorkey_R, Colorkey_G, Colorkey_B));
        new_Texture = SDL_CreateTextureFromSurface(renderer, loadedsurface);
        if (new_Texture != NULL) {
            rect_.w = loadedsurface->w;
            rect_.h = loadedsurface->h;
        }
        SDL_FreeSurface(loadedsurface);
    }
    myobject = new_Texture;
    return myobject != NULL;
}

void Object::Render(SDL_Renderer *des, const SDL_Rect *clip) {
    SDL_Rect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h};
    SDL_RenderCopy(des, myobject, clip, &renderquad);
}

void Object::Free() {
    if (myobject != NULL) {
        SDL_DestroyTexture(myobject);
        myobject = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}