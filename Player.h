#ifndef SASUKE_H_
#define SASUKE_H_

#include "SDL_Init.h"
#include "Object.h"
#include "mapgame.h"

#define GRAVITY 1
#define MAX_FAIL 8
#define SPEED 10
#define JUMPSPEED 2
#define MAXJUMP 2*TILE_SIZE

class Player : public Object
{
public:
	Player();
	~Player();
	enum Direction
	{
		Right = 0,
		Left = 1,
	};
	bool LoadImg(string path, SDL_Renderer* renderer);
	void Set_Frame();
	void Present(SDL_Renderer* des);
	void InputAction(SDL_Event events, SDL_Renderer* renderer);
	void Move(Map& mymap);
	void CheckVaCham(Map& mymap);
	void SetMoveMap(int x_map_, int y_map_) { x_map = x_map_; y_map = y_map_; };
	void MoveMap(Map& mymap);
	void Attacked();
	void Collect_Point(Map& mymap);
	bool check_win();
	void Open_Chain(Map& mymap);

	double x_value;
	double y_value;
	double x_pos;
	double y_pos;
	int width_character;
	int height_character;
	SDL_Rect gif[30];
	Input type_input;
	int CurrentIMG;
	int Direction;
	bool Stand_on_ground;
	int x_map;
	int y_map;
	int y_ground;
	bool death;
	int attack_count;
	bool Is_Attacked_Left;
	bool Is_Attackef_Right;
	int collected_point;
	int HP;
	int attacked_by;
	bool changeHealthBar;
	int HPDown;
};

#endif
