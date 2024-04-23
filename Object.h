#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>
#include <SDL.h>
#include <SDL_image.h>

class Object {
public:
    Object();
    ~Object();

    bool LoadImage(std::string path, SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer, const SDL_Rect* clip = nullptr);
    void Free();

    SDL_Rect GetRect() const { return rect_; }
    SDL_Texture* GetTexture() const { return texture_; }

private:
    SDL_Texture* texture_;
    SDL_Rect rect_;
    const int kColorKeyRed = 0;
    const int kColorKeyGreen = 0;
    const int kColorKeyBlue = 0;
};

#endif // OBJECT_H_