#include "Object.h"
#include "SDL_Init.h"

using namespace std;

Object::Object() {
    texture_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}

Object::~Object() {
    Free();
}

bool Object::LoadImage(string path, SDL_Renderer *renderer) {
    SDL_Texture *new_texture = NULL;
    SDL_Surface *loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface != NULL) {
        SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, kColorKeyRed, kColorKeyGreen, kColorKeyBlue));
        new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        if (new_texture != NULL) {
            rect_.w = loaded_surface->w;
            rect_.h = loaded_surface->h;
        }
        SDL_FreeSurface(loaded_surface);
    }
    texture_ = new_texture;
    return texture_ != NULL;
}

void Object::Render(SDL_Renderer *renderer, const SDL_Rect *clip) {
    SDL_Rect render_quad = {rect_.x, rect_.y, rect_.w, rect_.h};
    SDL_RenderCopy(renderer, texture_, clip, &render_quad);
}

void Object::Free() {
    if (texture_ != NULL) {
        SDL_DestroyTexture(texture_);
        texture_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}