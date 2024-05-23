#include "SDL_Init.h"

class Time {
public:
    Time();
    ~Time();
    
    void start();
    void stop();
    void pause();
    void resume();
    int getTime();
    bool isGameRunning();
    bool isGamePaused();
    int getTimePlayed();

private:
    Uint32 startTime;
    Uint32 gameStartTime;
    Uint32 totalTime;
    bool isRunning;
    bool isPaused;
};

Time::Time() {
    startTime = 0;
    gameStartTime = 0;
    totalTime = 0;
    isRunning = false;
    isPaused = false;
}

Time::~Time() {}

void Time::start() {
    isRunning = true;
    isPaused = false;
    startTime = SDL_GetTicks();
    gameStartTime = startTime;
}

void Time::stop() {
    isRunning = false;
    isPaused = false;
}

void Time::pause() {
    if (isRunning && !isPaused) {
        isPaused = true;
        totalTime += SDL_GetTicks() - startTime;
    }
}

void Time::resume() {
    if (isPaused) {
        isPaused = false;
        startTime = SDL_GetTicks();
    }
}

int Time::getTime() {
    if (isRunning) {
        if (isPaused) return totalTime;
        else return SDL_GetTicks() - startTime + totalTime;
    }
    return 0;
}

bool Time::isGameRunning() {
    return isRunning;
}

bool Time::isGamePaused() {
    return isPaused;
}

int Time::getTimePlayed() {
    return SDL_GetTicks() - gameStartTime;
}
