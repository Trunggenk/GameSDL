#include "Time.h"
#include "SDLFunc.h"

Time::Time() {
    begin_t = 0;
    paused_t = 0;
    start_tim = 0;
    sum_time = 0;
    paused_ = false;
    started_ = false;
}

Time::~Time() {

}

void Time::run_game() {
    start_tim = SDL_GetTicks();
}

void Time::begin() {
    started_ = true;
    paused_ = false;
    begin_t = SDL_GetTicks();
}

void Time::end() {
    paused_ = false;
    started_ = false;
}

void Time::pause() {
    if (started_ == true && paused_ == false) {
        paused_ = true;
        start_tim = begin_t;
        paused_t = SDL_GetTicks() - begin_t;
    }
}

void Time::unpause() {
    if (paused_ == true) {
        paused_ = false;
        begin_t = SDL_GetTicks() - paused_t;
        sum_time += paused_t;
        paused_t = 0;
    }
}

int Time::get_time() {
    if (started_ == true) {
        if (paused_ == true) return paused_t;
        else {
            return SDL_GetTicks() - begin_t;
        }
    }
    return 0;
}

bool Time::game_started() {
    return started_;
}

bool Time::game_paused() {
    return paused_;
}

int Time::time_played() {
    return SDL_GetTicks() - start_tim - sum_time;
}