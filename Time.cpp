#include "Time.h"
#include "SDL_Init.h"

Time::Time()
{
	begin_time = 0;
	paused_time = 0;
	time_start = 0;
	sum_paused_time = 0;
	paused_ = false;
	started_ = false;
}

Time::~Time()
{

}

void Time::run_game()
{
	time_start = SDL_GetTicks();
}

void Time::begin()
{
	started_ = true;
	paused_ = false;
	begin_time = SDL_GetTicks();
}

void Time::end()
{
	paused_ = false;
	started_ = false;
}

void Time::pause()
{
	if (started_ == true && paused_ == false)
	{
		paused_ = true;
		time_start = begin_time;
		paused_time = SDL_GetTicks() - begin_time;
	}
}

void Time::unpause()
{
	if (paused_ == true)
	{
		paused_ = false;
		begin_time = SDL_GetTicks() - paused_time;
		sum_paused_time += paused_time;
		paused_time = 0;
	}
}

int Time::get_time()
{
	if (started_ == true)
	{
		if (paused_ == true ) return paused_time;
		else
		{
			return SDL_GetTicks() - begin_time;
		}
	}
	return 0;
}

bool Time::game_started()
{
	return started_;
}

bool Time::game_paused()
{
	return paused_;
}

int Time::time_played()
{
	return SDL_GetTicks() - time_start - sum_paused_time;
}