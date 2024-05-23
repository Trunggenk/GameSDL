#ifndef TEXT_HANDLER_H
#define TEXT_HANDLER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class TextHandler {
public:
    TextHandler();
    ~TextHandler();

    bool LoadFromRenderText(TTF_Font *font, SDL_Renderer *renderer);
    void Free();
    void SetColor(Uint8 red, Uint8 green, Uint8 blue);
    void SetColor(int type);
    void RenderText(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = nullptr, double angle = 0.0,
                    SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
    SDL_Color text_color_;
    SDL_Texture *texture_;
    std::string str_val_;
    int width_;
    int height_;
};

#endif /* TEXT_HANDLER_H */
