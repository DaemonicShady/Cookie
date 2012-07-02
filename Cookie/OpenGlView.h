#ifndef __OpenGlView__
#define __OpenGlView__

#include <Windows.h>

#include "View.h"

class Mouse;
class Keyboard;

class OpenGlView : public View
{
public:
	OpenGlView ( string name, HINSTANCE hInstance,
				 int width, int height, int bpp,
				 bool fullscreen = false ); //!< Creates simple clear window.
	~OpenGlView ();

	virtual void   lock (); //!< Clears all buffers.
	virtual void   unlock (); //!< Swaps the framebuffers and draw new scene.
	virtual void   initialize (); //!< Initializes OpenGL basic parameters: clearcolor, hints, memory etc.

	virtual void   setFullScreen ( bool fullscreen );
	virtual void   resize ( int width, int height ); //!< Destroys and creates a new window with requested size. 

	HWND      getHWnd ();
	HINSTANCE getHInstance ();
	HDC       getHDc  ();

protected:
	bool changeResolution ( int width, int height, int bpp );
	void reshape          ( int width, int height );
	bool createWindow     ( int width, int height, int bpp, bool fullscreen );
	bool killWindow       ();

private:
	HINSTANCE hInstance_;
	HWND      hWnd_;
	HDC       hDC_;
	HGLRC     hRC_;

	Mouse*    mouse_;
	Keyboard* keyboard_;

	friend static LRESULT CALLBACK wndProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};

#endif
