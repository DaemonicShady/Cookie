#ifndef __FRAPS__
#define __FRAPS__

#include "Timer.h"

class Fraps : public Timer
{	
public:
	Fraps ( float interval = 1.0f );

	float secondsPerFrame ();
	float framesPerSecond ();

private:
	int    nFrames_;
	float  previousTime_;
	float  interval_;
	float  fps_;

	float  nextFrame ();
};

#endif