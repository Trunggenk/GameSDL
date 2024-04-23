#ifndef OBJECT_H_
#define OBJECT_H_

#include "SDL_Init.h"

using namespace std;

class Object {
protected:
    SDL_Texture *myobject;
    SDL_Rect rect_;
public:
    Object();

    ~Object();

    void SetRect(int x, int y) {
        rect_.x = x;
        rect_.y = y;
    }

    SDL_Rect GetRect() const { return rect_; }

    SDL_Texture *GetObject() const { return myobject; }

    virtual bool LoadImg(string path, SDL_Renderer *renderer);

    void Render(SDL_Renderer *des, const SDL_Rect *clip);

    virtual void Free();
};

#endif
