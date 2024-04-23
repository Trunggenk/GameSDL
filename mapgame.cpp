#include "mapgame.h"

using namespace std;


void MapGame::Load_Map() {
    const char *name = "map/map.dat";
    FILE *fileptr = NULL;
    fopen_s(&fileptr, name, "rb");
    if (fileptr == NULL) {
        cout << "Cannot open file map" << endl;
        return;
    }

    map_.pos_x_ = 0;
    map_.pos_y_ = 0;
    map_.start_x_ = 0;
    map_.start_y_ = 0;
    map_.mapname = name;

    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
            fscanf_s(fileptr, "%d", &map_.tile[i][j]);
            int value = map_.tile[i][j];
            if (value != 0) {
                if (j > map_.pos_x_) {
                    map_.pos_x_ = j;
                }
                if (i > map_.pos_y_) {
                    map_.pos_y_ = i;
                }
            }
        }
    }

    map_.pos_x_ = (map_.pos_x_ + 1) * TILE_SIZE;
    map_.pos_y_ = (map_.pos_y_ + 1) * TILE_SIZE;

    fclose(fileptr);
}


void MapGame::Load_Tiles(SDL_Renderer *renderer) {
    char img[50];
    FILE *fileptr = NULL;
    for (int i = 0; i < MAX_TILES; i++) {
        if (i == 4) {
            sprintf_s(img, "map/%d.gif", i);
        } else {
            sprintf_s(img, "map/%d.png", i);
        }
        fopen_s(&fileptr, img, "rb");
        if (fileptr == NULL) {
            continue;
        }
        fclose(fileptr);
        tile_map[i].LoadImg(img, renderer);
    }
}

void MapGame::Draw_Map(SDL_Renderer *renderer) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    int maploca_x_ = map_.start_x_ / TILE_SIZE;
    int maploca_y_ = map_.start_y_ / TILE_SIZE;

    x1 = -(map_.start_x_ % TILE_SIZE);
    if (x1 != 0) {
        x2 = x1 + SCREEN_WIDTH + TILE_SIZE;
    } else x2 = x1 + SCREEN_WIDTH;

    y1 = -(map_.start_y_ % TILE_SIZE);
    if (y1 != 0) {
        y2 = y1 + SCREEN_HEIGHT + TILE_SIZE;
    } else y2 = y1 + SCREEN_HEIGHT;

    for (int i = y1; i < y2; i += TILE_SIZE) {
        maploca_x_ = map_.start_x_ / TILE_SIZE;
        for (int j = x1; j < x2; j += TILE_SIZE) {
            int value = map_.tile[maploca_y_][maploca_x_];
            if (value != 0) {
                tile_map[value].SetRect(j, i);
                tile_map[value].Render(renderer, NULL);
            }
            maploca_x_++;
        }
        maploca_y_++;
    }
}