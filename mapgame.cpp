#include <iostream>
#include <cstdio>
#include "mapgame.h"

void MapGame::LoadMapData() {
    const char *mapFileName = "map/mapdata.dat";
    FILE *mapFile = fopen(mapFileName, "rb");
    if (!mapFile) {
        std::cerr << "Error: Unable to open map data file: " << mapFileName << std::endl;
        return;
    }

    map_.playerPosX = 0;
    map_.playerPosY = 0;
    map_.startPosX = 0;
    map_.startPosY = 0;
    map_.mapName = mapFileName;

    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_X; ++j) {
            int tileValue;
            if (fscanf(mapFile, "%d", &tileValue) != 1) {
                std::cerr << "Error: Unable to read tile data from map file: " << mapFileName << std::endl;
                fclose(mapFile);
                return;
            }
            map_.tiles[i][j] = tileValue;
            if (tileValue != 0) {
                map_.playerPosX = std::max(map_.playerPosX, j);
                map_.playerPosY = std::max(map_.playerPosY, i);
            }
        }
    }

    map_.playerPosX = (map_.playerPosX + 1) * TILE_SIZE;
    map_.playerPosY = (map_.playerPosY + 1) * TILE_SIZE;

    fclose(mapFile);
}

void MapGame::LoadTileImages(SDL_Renderer *renderer) {
    char imgFilename[50];
    for (int i = 0; i < MAX_TILES; ++i) {
        sprintf(imgFilename, "map/tile_%d.png", i);
        FILE *imgFile = fopen(imgFilename, "rb");
        if (!imgFile) {
            continue;
        }
        fclose(imgFile);
        tileImages[i].LoadImage(imgFilename, renderer);
    }
}

void MapGame::RenderMap(SDL_Renderer *renderer) {
    int viewportLeft = 0, viewportRight = 0, viewportTop = 0, viewportBottom = 0;

    int mapLocationX = map_.startPosX / TILE_SIZE;
    int mapLocationY = map_.startPosY / TILE_SIZE;

    viewportLeft = -(map_.startPosX % TILE_SIZE);
    viewportRight = viewportLeft != 0 ? viewportLeft + SCREEN_WIDTH + TILE_SIZE : viewportLeft + SCREEN_WIDTH;

    viewportTop = -(map_.startPosY % TILE_SIZE);
    viewportBottom = viewportTop != 0 ? viewportTop + SCREEN_HEIGHT + TILE_SIZE : viewportTop + SCREEN_HEIGHT;

    for (int i = viewportTop; i < viewportBottom; i += TILE_SIZE) {
        mapLocationX = map_.startPosX / TILE_SIZE;
        for (int j = viewportLeft; j < viewportRight; j += TILE_SIZE) {
            int tileValue = map_.tiles[mapLocationY][mapLocationX];
            if (tileValue != 0) {
                tileImages[tileValue].SetPosition(j, i);
                tileImages[tileValue].Render(renderer, nullptr);
            }
            ++mapLocationX;
        }
        ++mapLocationY;
    }
}
