#ifndef TIME_MANAGE_H_
#define TIME_MANAGE_H_

class Time
{
public:
	Time();
	~Time();
	void run_game();
	void begin();
	void end();
	void pause();
	void unpause();
	int get_time();
	bool game_started();
	bool game_paused();
	int time_played();
private:
	int begin_time;
	int paused_time;
	bool paused_;
	bool started_;
	int time_start;
	int sum_paused_time;

};

#endif
