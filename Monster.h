#ifndef MONSTER_H_
#define MONSTER_H_

#include "SDL_Init.h"
#include "Object.h"
#include "Player.h"

#define GRAVITY 1
#define MAX_FAIL 8
#define MON_SPEED 4
#define MON_JUMPSPEED 2
#define MAXJUMP 2*TILE_SIZE

class Monster : public Object
{
public:
	Monster();
	~Monster(); 
	enum Direction
	{
		Right = 0,
		Left = 1,
	};
	void get_monster(const int& type) { type_monster = type; }
	void set_x_val(const double& xVal) { x_value = xVal; }
	void set_y_val(const double& yval) { y_value = yval; }
	void set_x_pos(const double& xpos) { x_pos = xpos; }
	void set_y_pos(const double& ypos) { y_pos = ypos; }
	void set_type(const int& tp) { type_monster = tp; }
	double get_x_pos() const { return x_pos; }
	double get_y_pos() const { return y_pos; }
	void SetMapXY(const int& mp_x, const int& mp_y) { x_map = mp_x; y_map = mp_y; }
	void set_clips();
	bool LoadImg(std::string path, SDL_Renderer* scree);
	void Present(SDL_Renderer* des);
	int get_width_frame() const { return width_mon; }
	int get_height_frame() const { return height_mon; }
	void MonsterMove(Map& mymap);
	void CheckVaCham(Map& mymap);
	bool Meet_Sasuke_( const Map mymap);
	void Action(Player sake);
	void MoveAround();
	void set_Mid(double const& xpos) { Mid = xpos; };
	double get_Mid() const { return Mid; }
	void set_range(double const& xpos);
	double get_begin() { return begin_move; }
	double get_end() { return end_move; }
	//void manage_range(Player sake);
	void set_Mon_HP(int type);
	void Is_Attacked(Player sake);
	void Attack(Player& sake);
	bool Is_Dead();

private:
	int type_monster;
	int x_map;
	int y_map;
	double x_pos;
	double y_pos;
	double x_value;
	double y_value;
	int width_mon;
	int height_mon;
	int CurrentIMG;
	SDL_Rect gif[30];
	bool Stand_on_ground;
	int Direction;
	Input type_input;
	bool Meet_Sasuke;
	double Mid;
	double y_ground;
	double begin_move;
	double end_move;
	double Mon_HP;
	bool death;
	bool isattacked;
	bool attack;
	bool del;
};

#endif
