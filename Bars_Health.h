#ifndef Bars_Health
#define Bars_Health

#include "Player.h"

class Bars : public Object {
public:

    Bars();

    ~Bars();

    bool Loadimage(string path, SDL_Renderer *renderer);

    void S_Frame();

    void Display(Player sake, SDL_Renderer *des);

    enum type {
        Health = 0,
        Mana = 1,
    };

    void set_type(int _type) { type = _type; }

private:
    int barWidth;
    int barHeight;
    SDL_Rect gif[30];
    int currentFrame;
    int mapX;
    int mapY;
    int type;
};


#endif