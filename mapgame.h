#ifndef MAP_GAME_H_
#define MAP_GAME_H_

#include "SDLFunc.h"
#include "Object.h"

#define MAX_TILES 10

using namespace std;

class TileMat : public Object {
public:
    TileMat() { ; }

    ~TileMat() { ; }
};

class MapGame {
    currentMap map_;
    TileMat tile_map[MAX_TILES];
public:
    MapGame() {}

    ~MapGame() {}

    currentMap GetMap() { return map_; }

    void SetMap(currentMap mymap) { map_ = mymap; };

    void Load_Map();

    void Load_Tiles(SDL_Renderer *renderer);

    void Draw_Map(SDL_Renderer *renderer);
};

#endif
