#include "Text.h"

TextHandler::TextHandler() {
    text_color_ = {255, 255, 255};
    texture_ = nullptr;
}

TextHandler::~TextHandler() {
    Free();
}

bool TextHandler::LoadFromRenderText(TTF_Font *font, SDL_Renderer *renderer) {
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, str_val_.c_str(), text_color_);
    if (text_surface) {
        texture_ = SDL_CreateTextureFromSurface(renderer, text_surface);
        if (texture_) {
            width_ = text_surface->w;
            height_ = text_surface->h;
        }
        SDL_FreeSurface(text_surface);
    }
    return texture_ != nullptr;
}

void TextHandler::Free() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

void TextHandler::SetColor(Uint8 red, Uint8 green, Uint8 blue) {
    text_color_ = {red, green, blue};
}

void TextHandler::SetColor(int type) {
    switch (type) {
        case RED_TEXT:
            text_color_ = {180, 0, 0};
            break;
        case WHITE_TEXT:
            text_color_ = {255, 255, 255};
            break;
        case BLACK_TEXT:
            text_color_ = {0, 0, 0};
            break;
        default:
            break;
    }
}

void TextHandler::RenderText(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip, double angle, SDL_Point *center,
                             SDL_RendererFlip flip) {
    SDL_Rect render_quad = {x, y, width_, height_};
    if (clip) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }
    SDL_RenderCopyEx(renderer, texture_, clip, &render_quad, angle, center, flip);
}
