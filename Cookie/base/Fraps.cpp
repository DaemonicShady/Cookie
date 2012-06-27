#include "Fraps.h"

Fraps::Fraps ( float interval )
{
	previousTime_ = 0.0f;
	interval_     = interval;
	fps_          = 0.0f;
	nFrames_      = 0;
}

float Fraps::secondsPerFrame ()
{
	nextFrame ();

	return ( fps_ == 0.0f ) ? 0.0f : 1.0f / fps_;
}

float Fraps::framesPerSecond ()
{
	return nextFrame ();
}

float Fraps::nextFrame ()
{
	nFrames_++;

	if ( previousTime_ == 0.0f )
		previousTime_ = getTime ();

	float currentTime = getTime ();
	float difference  = currentTime - previousTime_;

	if ( difference >= interval_ ) {
		previousTime_ = currentTime;
		fps_          = (float)nFrames_ / difference;
		nFrames_      = 0;
	}

	return fps_;
}