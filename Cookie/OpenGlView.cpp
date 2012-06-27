#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "OpenGlView.h"
#include "Controller.h"

static const char* className = "OpenGlViewClass";

OpenGlView::OpenGlView ( string name, HINSTANCE hInstance,
						 int width, int height,
						 int bpp, bool fullscreen ) : View ( name )
{
	hInstance_ = hInstance;
	hWnd_      = 0;
	hDC_       = 0;
	hRC_       = 0;

	WNDCLASSEX wc;

	ZeroMemory ( &wc, sizeof(WNDCLASSEX) );

	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)wndProc;
	wc.hInstance     = hInstance;
	wc.hbrBackground = 0; // (HBRUSH)COLOR_APPWORKSPACE;
	wc.hCursor       = LoadCursor ( NULL, IDC_ARROW );
	wc.hIcon         = LoadIcon   ( NULL, IDI_WINLOGO );
	wc.lpszClassName = className;
//	wc.cbClsExtra    = 0;
//	wc.cbWndExtra    = 0;

	if ( !RegisterClassEx ( &wc ) )
		return;
	
	createWindow ( width, height, bpp, fullscreen );
}

OpenGlView::~OpenGlView ()
{
	UnregisterClass ( className, hInstance_ );
	killWindow ();
}

bool OpenGlView::createWindow ( int width, int height, int bpp, bool fullscreen )
{
	DWORD windowStyle         = WS_OVERLAPPEDWINDOW;
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;
	RECT  windowRect          = { 0, 0, width, height };

	if ( fullscreen ) {
		windowStyle			= WS_POPUP;
		windowExtendedStyle = WS_EX_APPWINDOW;

		ShowCursor ( false );
	}
	else {
		windowStyle			= WS_OVERLAPPEDWINDOW;;
		windowExtendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	}

	if ( fullscreen ) {
		if ( !changeResolution ( width, height, bpp ) ) {
			//MessageBox ();
			fullscreen = false;
		}
		else {
			ShowCursor ( false );
			windowExtendedStyle |= WS_EX_TOPMOST;
		}
	}
	else
		AdjustWindowRectEx ( &windowRect, windowStyle, 0, windowExtendedStyle );

	hWnd_ = CreateWindowEx ( windowExtendedStyle,
							 className,
							 appName_.c_str(),
							 WS_CLIPSIBLINGS | WS_CLIPCHILDREN | windowStyle,
							 0,
							 0,
							 windowRect.right - windowRect.left,
							 windowRect.bottom - windowRect.top,
							 0,
							 0,
							 hInstance_, 
							 this );

	if ( hWnd_ == 0 )
		return false;

	hDC_ = GetDC ( hWnd_ );

	if ( hDC_ == 0 )
		return !killWindow ();

	PIXELFORMATDESCRIPTOR pfd =									
	{
		sizeof (PIXELFORMATDESCRIPTOR),							// size of this Pixel Format Descriptor
		1,														// version number
		PFD_DRAW_TO_WINDOW |									// format must support Window
		PFD_SUPPORT_OPENGL |									// format must support OpenGL
		PFD_DOUBLEBUFFER,										// must support double buffering
		PFD_TYPE_RGBA,											// request an RGBA format
		bpp,													// select color depth
		0, 0, 0, 0, 0, 0,										// color bits
		0,														// no alpha buffer
		0,														// shift bit ignored
		0,														// no accumulation cuffer
		0, 0, 0, 0,												// accumulation bits ignored
		32,														// Z-Buffer depth
		0,														// 8-bit stencil buffer
		0,														// no auxiliary buffer
		PFD_MAIN_PLANE,											// main drawing layer
		0,														// reserved
		0, 0, 0													// layer masks ignored
	};

	unsigned int pixelFormat;

	pixelFormat = ChoosePixelFormat ( hDC_, &pfd );

	if ( pixelFormat == 0 )
		return !killWindow ();

	if ( SetPixelFormat ( hDC_, pixelFormat, &pfd ) == false )
		return !killWindow ();

	hRC_ = wglCreateContext ( hDC_ );

	if ( hRC_ == 0 )
		return !killWindow ();

	if ( wglMakeCurrent ( hDC_, hRC_ ) == false )
		return !killWindow ();

	bitsPerPixel_ = bpp;
	fullscreen_	  = fullscreen;

	reshape	   ( width, height );
	initialize ();
	ShowWindow ( hWnd_, SW_SHOW );
	
	return true;
}

bool OpenGlView::killWindow ()
{
	if ( fullscreen_ )
		ShowCursor ( true );

	wglMakeCurrent ( 0, 0 );

	if ( hRC_ != 0 )
		wglDeleteContext ( hRC_ );

	if ( hDC_ != 0 )
		ReleaseDC ( hWnd_, hDC_ );

	if ( hWnd_ != 0 )
		DestroyWindow ( hWnd_ );

	hWnd_ = 0;
	hDC_  = 0;
	hRC_  = 0;

	ChangeDisplaySettings ( 0, 0 );

	return true;
}

bool OpenGlView::changeResolution ( int width, int height, int bpp )
{
	DEVMODE dmScreenSettings;

	dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth  = width;
	dmScreenSettings.dmPelsHeight = height;
	dmScreenSettings.dmBitsPerPel = bpp;
	dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	if ( ChangeDisplaySettings ( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
		return false;

	width_        = width;
	height_       = height;
	bitsPerPixel_ = bpp;

	return true;
}

void OpenGlView::setFullScreen ( bool fullscreen )
{
	if ( fullscreen_ == fullscreen )
		return;

	killWindow   ();
	createWindow ( width_, height_, bitsPerPixel_, fullscreen );
}

void OpenGlView::resize ( int width, int height )
{
	killWindow   ();
	createWindow ( width, height, bitsPerPixel_, fullscreen_ );
}

/****************** OpenGL functions **********************/

void OpenGlView::lock ()
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

void OpenGlView::unlock ()
{
	SwapBuffers ( hDC_ );
}

void OpenGlView::initialize ()
{
	glClearColor	( 1.0f, 1.0f, 1.0f, 1.0f );
	glClearDepth	( 1.0 );
	glClearStencil	( 0 );
	glHint			( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
//	glPixelStorei	( GL_PACK_ALIGNMENT, 1 );
//	glPixelStorei	( GL_UNPACK_ALIGNMENT, 1 );
}

void OpenGlView::reshape ( int width, int height )
{
	width_  = width;
	height_ = height;

	glViewport ( 0, 0, width, height );

	glMatrixMode	( GL_PROJECTION );
	glLoadIdentity	();
	gluPerspective	( 45.0, (GLdouble)width/height, 2.0, 100.0 );
}

/****************** Window functions **********************/

static LRESULT CALLBACK wndProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	OpenGlView* view = (OpenGlView *)GetWindowLongPtr ( hWnd, GWL_USERDATA );

	switch ( msg ) {
		case WM_SYSCOMMAND :

			switch ( wParam ) {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}
			break;

		case WM_COMMAND :
			return 0;

		case WM_CREATE :
		{
			CREATESTRUCT *creation = (CREATESTRUCT *)lParam;

			view = (OpenGlView *)creation -> lpCreateParams;
			SetWindowLong ( hWnd, GWL_USERDATA, (LONG)view );

			return 0;
		}
			
		case WM_CLOSE :
			PostMessage ( hWnd, WM_QUIT, 0, 0 );
			return 0;

		case WM_KEYDOWN :
			view -> getController () -> keyboard ( wParam, true );
			return 0;

		case WM_KEYUP :
			view -> getController () -> keyboard ( wParam, false );
			return 0;

		case WM_LBUTTONDOWN :
			view -> getController () -> mouseClick ( lParam, true );
			return 0;

		case WM_SIZE :
			view -> reshape ( LOWORD(lParam), HIWORD(lParam) );
			view -> getController () -> draw ();
			return 0;

		case WM_ACTIVATE :
			return 0;

		default :
			break;
	}

	return DefWindowProc ( hWnd, msg, wParam, lParam );
}

/****************Setters & Getters***********************/
HWND OpenGlView::getHWnd ()
{
	return hWnd_;
}

HDC OpenGlView::getHDc ()
{
	return hDC_;
}

/****************HELPERS***********************/
