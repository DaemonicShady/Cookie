#ifndef __VIEW__
#define __VIEW__

#include <Windows.h>
#include <string>

class Controller;

using std::string;

class View
{
public:
	void        setController ( Controller* controller );
	Controller* getController () const;

	virtual void lock       () = 0; //!< Clears all buffers.
	virtual void unlock     () = 0; //!< Swaps the framebuffers and draws new scene.
	virtual void initialize () = 0; //!< Initializes OpenGL basic parameters: clearcolor, hints, memory etc.

	virtual void setFullScreen ( bool fullscreen ) = 0;
	
	virtual void resize        ( int width, int height ) = 0; //!< Destroys and creates a new window with requested size.

	virtual void startOrtho () {};
	virtual void endOrtho   () {};

	virtual HWND getHWnd () = 0;
	virtual HDC  getHDc  () = 0;

	int     getWidth  () const;
	int     getHeight () const;

	~View () {} 
protected:
	int         width_;
	int         height_;
	int         bitsPerPixel_;
	bool        fullscreen_;
	string      appName_;
	Controller* controller_;

	View ( string appName );
};

#endif