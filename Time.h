#ifndef TIME_MANAGE_H_
#define TIME_MANAGE_H_

class Time {
public:
    Time();
    ~Time();


    void begin();
    void run_game();


    void pause();
    void end();
    int get_time();

    void unpause();


    bool game_started();

    bool game_paused();

    int time_played();

private:
    int startTime;
    int pausedTime;
    bool isPaused;
    bool isStarted;
    int gameStartTime;
    int totalTime;

};

#endif
