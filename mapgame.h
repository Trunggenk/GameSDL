#ifndef MAPGAME_H
#define MAPGAME_H

#include <SDL2/SDL.h>
#include <string>
#include <algorithm>

const int MAX_X = 100;
const int MAX_Y = 100;
const int TILE_SIZE = 32;
const int MAX_TILES = 256;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

struct Map {
    int playerPosX;
    int playerPosY;
    int startPosX;
    int startPosY;
    std::string mapName;
    int tiles[MAX_Y][MAX_X];
};

class TileImage {
public:
    void LoadImage(const std::string& filename, SDL_Renderer* renderer);
    void SetPosition(int x, int y);
    void Render(SDL_Renderer* renderer, SDL_Rect* clip);
    // Add other necessary member functions and data members
};

class MapGame {
public:
    void LoadMapData();
    void LoadTileImages(SDL_Renderer* renderer);
    void RenderMap(SDL_Renderer* renderer);

private:
    Map map_;
    TileImage tileImages[MAX_TILES];
};

#endif // MAPGAME_H
