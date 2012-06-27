#include "Timer.h"

Timer::Timer ()
{
	startTime_  = timeGetTime ();
	pausedTime_ = 0.0f;
	paused_     = 0;
}

float Timer::getTime () const
{
	return abs( int(timeGetTime () - startTime_) ) * 0.001f - pausedTime_;
}

void Timer::pause ()
{
	if ( ++paused_ == 1 )
		startPause_ = timeGetTime ();
}

void Timer::resume ()
{
	if ( paused_ <= 0 )
		return;

	if ( --paused_ == 0 )
		pausedTime_ += abs( int(timeGetTime () - startPause_) ) * 0.001f;
}
