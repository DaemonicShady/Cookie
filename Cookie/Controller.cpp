#include <Windows.h>
#include <gl\gl.h>
#include <RasterFont.h>

#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "base\Fraps.h"

Controller::Controller ( Model* model, View* view )
{
	view_  = view;
	model_ = model;
	view_ -> setController ( this );

	for ( int i = 0; i < 256; i++ )
		keys [i] = false;

	fraps_ = new Fraps;
	font_  = new RasterFont ( view -> getHDc (), 16, "Comic Sans MC" );
	font_ -> setPosition ( -0.99f, 0.94f );
}

void Controller::keyboard ( char key, bool isPressed )
{
	keys [key] = isPressed;
}

bool Controller::isPressed ( char key )
{
	return keys [key];
}

void Controller::mouseClick ( int param, bool isPressed )
{
	POINT mouse;

	if ( isPressed ) {
		mouse.x = LOWORD (param);
		mouse.y = HIWORD (param);
	}
}

int Controller::update ()
{
	if ( keys [VK_ESCAPE] == true )
		return CTRL_QUIT;

	view_  -> lock   ();
	model_ -> render ();
	draw ();
	view_  -> unlock ();

	return CTRL_CONTINUE;
}

void Controller::draw ()
{
	float fps = fraps_ -> framesPerSecond ();

	glMatrixMode   ( GL_MODELVIEW );
	glLoadIdentity ();
	glMatrixMode   ( GL_PROJECTION );
	glPushMatrix   ();
	glLoadIdentity ();
	glOrtho        ( -1, 1, -1, 1, 0, 100 );
	glColor3f      ( 1.0f, 0.0f,  1.0f );
	font_ -> print ( "fps = %f", fps );
	glPopMatrix    ();
}
