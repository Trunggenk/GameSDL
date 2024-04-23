#include "Time.h"
#include "SDL_Init.h"

Time::Time() {
    startTime = 0;
    pausedTime = 0;
    gameStartTime = 0;
    totalTime = 0;
    isPaused = false;
    isStarted = false;
}

Time::~Time() {

}

void Time::run_game() {
    gameStartTime = SDL_GetTicks();
}

void Time::begin() {
    isStarted = true;
    isPaused = false;
    startTime = SDL_GetTicks();
}

void Time::end() {
    isPaused = false;
    isStarted = false;
}

void Time::pause() {
    if (isStarted == true && isPaused == false) {
        isPaused = true;
        gameStartTime = startTime;
        pausedTime = SDL_GetTicks() - startTime;
    }
}

void Time::unpause() {
    if (isPaused == true) {
        isPaused = false;
        startTime = SDL_GetTicks() - pausedTime;
        totalTime += pausedTime;
        pausedTime = 0;
    }
}

int Time::get_time() {
    if (isStarted == true) {
        if (isPaused == true) return pausedTime;
        else {
            return SDL_GetTicks() - startTime;
        }
    }
    return 0;
}

bool Time::game_started() {
    return isStarted;
}

bool Time::game_paused() {
    return isPaused;
}

int Time::time_played() {
    return SDL_GetTicks() - gameStartTime - totalTime;
}