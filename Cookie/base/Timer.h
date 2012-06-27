#ifndef __TIMER__
#define __TIMER__

#include <windows.h>

class Timer
{
public:
	Timer ();

	float getTime () const;

	void  pause   ();
	void  resume  ();

protected:
	int   paused_;

private:
	DWORD startTime_;
	DWORD startPause_;
	float pausedTime_;
};

#endif