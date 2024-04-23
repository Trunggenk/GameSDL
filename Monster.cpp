#include "Monster.h"

int frame_number_monster(int type, Input in) {
    if (type == 1) {
        if (in.jump == 1) return 1;
        if (in.fall == 1) return 1;
        if (in.run == 1) return 8;
        if (in.normal_attack == 1) return 8;
    } else if (type == 3) {
        if (in.jump == 1) return 1;
        if (in.fall == 1) return 1;
        if (in.run == 1) return 4;
        if (in.normal_attack == 1) return 4;
    }
}

void Monster::set_Mon_HP(int type) {
    if (type == 1) Mon_HP = 500;
    else if (type == 3) Mon_HP = 1000;
}

Monster::Monster() {
    x_map = 0;
    y_map = 0;
    x_pos = 0.0;
    y_pos = 0.0;
    x_value = 0.0;
    y_value = 0.0;
    width_mon = 0;
    height_mon = 0;
    CurrentIMG = 0;
    Stand_on_ground = false;
    Direction = Left;
    type_input.run = 1;
    type_input.jump = 0;
    type_input.normal_attack = 0;
    type_input.fall = 0;
    Meet_Sasuke = false;
    Mid = 0;
    Mon_HP = 0;
    death = false;
    isattacked = false;
    attack = false;
    del = false;
}

Monster::~Monster() {

}

bool Monster::LoadImg(std::string path, SDL_Renderer *screen) {
    bool loadedimg = Object::LoadImg(path, screen);
    if (loadedimg) {
        width_mon = rect_.w / frame_number_monster(type_monster, type_input);
        height_mon = rect_.h;
    }
    return loadedimg;
}

void Monster::set_clips() {
    int frame_number_ = frame_number_monster(type_monster, type_input);
    for (int i = 0; i < frame_number_; i += 1) {
        gif[i].x = i * width_mon;
        gif[i].y = 0;
        gif[i].w = width_mon;
        gif[i].h = height_mon;
    }
}

void Monster::Present(SDL_Renderer *des) {
    if (type_monster == 1) {
        if (Direction == Left) {
            if (type_input.jump == 1) LoadImg("Monster/1/m1_jump_left.png", des);
            else if (type_input.fall == 1) LoadImg("Monster/1/m1_jump_left.png", des);
            else if (type_input.normal_attack == 1) LoadImg("Monster/1/m1_fight_left.png", des);
            else if (type_input.run == 1) LoadImg("Monster/1/m1_run_left.png", des);
        } else {
            if (type_input.jump == 1) LoadImg("Monster/1/m1_jump_right.png", des);
            else if (type_input.fall == 1) LoadImg("Monster/1/m1_jump_right.png", des);
            else if (type_input.normal_attack == 1) LoadImg("Monster/1/m1_fight_right.png", des);
            else if (type_input.run == 1) LoadImg("Monster/1/m1_run_right.png", des);
        }
        CurrentIMG++;
        if (CurrentIMG >= frame_number_monster(type_monster, type_input)) {
            CurrentIMG = 0;
            if (type_input.normal_attack == 1) type_input.normal_attack = 0;
        }
        rect_.x = x_pos - x_map;
        rect_.y = y_pos - y_map;
        SDL_Rect *current_clip = &gif[CurrentIMG];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_mon, height_mon};
        SDL_RenderCopy(des, myobject, current_clip, &renderQuad);
    } else if (type_monster == 3) {
        if (Direction == Left) {
            if (type_input.jump == 1) LoadImg("Monster/3/m3_jump_left.png", des);
            else if (type_input.fall == 1) LoadImg("Monster/3/m3_jump_left.png", des);
            else if (type_input.normal_attack == 1) LoadImg("Monster/3/m3_fight_left.png", des);
            else if (type_input.run == 1) LoadImg("Monster/3/m3_run_left.png", des);
        } else {
            if (type_input.jump == 1) LoadImg("Monster/3/m3_jump_right.png", des);
            else if (type_input.fall == 1) LoadImg("Monster/3/m3_jump_right.png", des);
            else if (type_input.normal_attack == 1) LoadImg("Monster/3/m3_fight_right.png", des);
            else if (type_input.run == 1) LoadImg("Monster/3/m3_run_right.png", des);
        }
        CurrentIMG++;
        if (CurrentIMG >= frame_number_monster(type_monster, type_input)) {
            CurrentIMG = 0;
            if (type_input.normal_attack == 1) type_input.normal_attack = 0;
        }
        rect_.x = x_pos - x_map;
        rect_.y = y_pos - y_map;
        SDL_Rect *current_clip = &gif[CurrentIMG];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_mon, height_mon};
        SDL_RenderCopy(des, myobject, current_clip, &renderQuad);
    }
}

void Monster::MonsterMove(currentMap &mymap) {
    x_value = 0;
    y_value += MON_JUMPSPEED;

    if (y_value >= MAX_FAIL) y_value = MAX_FAIL;
    if (type_input.run == 1 && Direction == Right) {
        x_value += MON_SPEED;
    } else if (type_input.run == 1 && Direction == Left) {
        x_value -= MON_SPEED;
    }
    if (type_input.jump == 1) {
        y_value -= MON_JUMPSPEED;
    } else if (type_input.fall == 1) {
        y_value += MON_JUMPSPEED;
    }
    if (type_input.normal_attack == 1 && type_input.run == 1) {
        if (Direction == Right) x_value -= SPEED / 3;
        else if (Direction == Left) x_value += SPEED / 3;
    }
    CheckVaCham(mymap);
}

void Monster::CheckVaCham(currentMap &mymap) {
    int x1 = (x_pos + x_value) / TILE_SIZE;
    int y1 = y_pos / TILE_SIZE;
    int x2 = (x_pos + x_value + TILE_SIZE) / TILE_SIZE;
    int y2 = (y_pos + TILE_SIZE) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_X && y1 >= 0 && y2 < MAX_Y) {
        if (x_value > 0) //di sang phai
        {
            if (mymap.tile[y1][x2] != 0 && mymap.tile[y2][x2] != 0) {
                x_pos = x2 * TILE_SIZE;
                x_pos -= TILE_SIZE;
                x_value = 0;
            }
        } else if (x_value < 0) {
            if (mymap.tile[y1][x1] != 0 && mymap.tile[y2][x1] != 0) {
                x_pos = (x1 + 1) * TILE_SIZE;
                x_value = 0;
            }
        }
    }

    x1 = (x_pos) / TILE_SIZE;
    y1 = (y_pos + y_value) / TILE_SIZE;
    x2 = (x_pos + TILE_SIZE) / TILE_SIZE;
    y2 = (y_pos + y_value + TILE_SIZE) / TILE_SIZE;

    if (!death) {
        if (x1 >= 0 && x2 < MAX_X && y1 >= 0 && y2 < MAX_Y) {
            if (y_value > 0) //roi xuong
            {
                if (mymap.tile[y2][x1] == 3 || mymap.tile[y2][x2] == 3) {
                    y_pos = y2 * TILE_SIZE;
                    y_pos -= TILE_SIZE;
                    y_value = 0;
                    Stand_on_ground = true;
                    type_input.fall = 0;
                } else { Stand_on_ground = false; }
            } else if (y_value < 0) {
                if (mymap.tile[y1][x1] != 0 || mymap.tile[y1][x2] != 0) {
                    y_pos = (y1 + 1) * TILE_SIZE;
                    y_value = 0;
                }
            }
        }
    } else {
        y_value = 10 * MON_JUMPSPEED;
        x_value = 0;
    }

    x_pos += x_value;
    y_pos += y_value;

    if (x_pos < 0) x_pos = 0;
    else if (x_pos + width_mon > MAX_X * TILE_SIZE) x_pos = MAX_X * TILE_SIZE - width_mon;
    if (y_pos < 0) y_pos = 0;
    else if (y_pos + height_mon > MAX_Y * TILE_SIZE) {
        y_pos = (MAX_Y + 1) * TILE_SIZE - height_mon;
        del = true;
    };
}

bool Monster::Meet_Sasuke_(const currentMap mymap) {
    if (x_pos <= mymap.start_x_ + SCREEN_WIDTH * 2 / 3 || Meet_Sasuke) {
        Meet_Sasuke = true;
        return true;
    }
    else return false;
}

void Monster::Action(Player sake) {
    if (y_pos == sake.y_pos) {
        if (sake.x_pos < x_pos && type_input.normal_attack == 0) {
            Direction = Left;
            type_input.run = 1;
        }
        else if (sake.x_pos >= x_pos && type_input.normal_attack == 0) {
            Direction = Right;
            type_input.run = 1;
        }
        if (sake.x_pos - TILE_SIZE <= x_pos && y_pos == sake.y_pos && Direction == Right) {
            type_input.normal_attack = 1;
            type_input.run = 1;
        } else if (sake.x_pos + 1 * TILE_SIZE >= x_pos && y_pos == sake.y_pos && Direction == Left) {
            type_input.normal_attack = 1;
            type_input.run = 1;
        }
    } else MoveAround();
}

void Monster::MoveAround() {
    double start = Mid - 2 * TILE_SIZE;
    double end = Mid + 2 * TILE_SIZE;
    if (x_pos <= start) Direction = Right;
    else if (x_pos >= end) Direction = Left;
}

void Monster::set_range(double const &xpos) {
    begin_move = xpos - 6 * TILE_SIZE;
    end_move = xpos + 6 * TILE_SIZE;
}

void Monster::Is_Attacked(Player sake) {
    if (sake.type_input.HoaDon == 1) {
        if (sake.x_pos - TILE_SIZE - x_pos > 0 && sake.x_pos - TILE_SIZE - x_pos < 0.5 * TILE_SIZE &&
            y_pos == sake.y_pos) {
            if (sake.Direction == Left) {
                type_input.normal_attack = 0;
                x_pos -= 0.5 * TILE_SIZE;
                SDL_Delay(20);
                Mon_HP -= 120;
            }
        } else if (sake.x_pos + TILE_SIZE - x_pos <= 0 && sake.x_pos + TILE_SIZE - x_pos >= -0.5 * TILE_SIZE &&
                   y_pos == sake.y_pos) {
            if (sake.Direction == Right) {
                type_input.normal_attack = 0;
                x_pos += 0.5 * TILE_SIZE;
                SDL_Delay(20);
                Mon_HP -= 120;
            }
        }
    } else if (sake.type_input.normal_attack == 1) {
        if (sake.x_pos - x_pos <= 0 && sake.x_pos - x_pos >= -0.5 * TILE_SIZE && y_pos == sake.y_pos) {
            if (sake.Direction == Right) {
                type_input.normal_attack = 0;
                x_pos += 0.5 * TILE_SIZE;
                SDL_Delay(20);
                Mon_HP -= 50;
            }
        } else if (sake.x_pos - x_pos > 0 && sake.x_pos - x_pos < 0.5 * TILE_SIZE && y_pos == sake.y_pos) {
            if (sake.Direction == Left) {
                type_input.normal_attack = 0;
                x_pos -= 0.5 * TILE_SIZE;
                SDL_Delay(20);
                Mon_HP -= 50;
            }
        }
    }
}

void Monster::Attack(Player &sake) {
    double attack_range;
    if (type_monster == 1) attack_range = 0.5 * TILE_SIZE;
    else if (type_monster == 3) attack_range = 0.5 * TILE_SIZE;
    if (x_pos - sake.x_pos >= 0 && x_pos - sake.x_pos <= attack_range && y_pos == sake.y_pos &&
        type_input.normal_attack == 1) {
        sake.Is_Attackef_Right = true;
    } else if (x_pos - sake.x_pos<0 && x_pos - sake.x_pos>(-1) * attack_range && y_pos == sake.y_pos &&
               type_input.normal_attack == 1) {
        sake.Is_Attacked_Left = true;
    }
    sake.attacked_by = type_monster;
}

bool Monster::Is_Dead() //xem lai
{
    if (Mon_HP <= 0) death = true;
    if (del) return true;
    else return false;
}



