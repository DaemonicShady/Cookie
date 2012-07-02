#include <Windows.h>
#include <gl\gl.h>
#include <RasterFont.h>

#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Camera.h"

#include "base\Fraps.h"
#include "base\Keyboard.h"
#include "base\Mouse.h"

Controller::Controller ( Model* model, View* view )
{
	view_   = view;
	model_  = model;
	view_  -> setController ( this );
	camera_ = new Camera;

	fraps_  = new Fraps;
	font_   = new RasterFont ( view -> getHDc (), 16, "Times New Roman" );
	font_  -> setPosition ( -0.99f, 0.94f );

	shouldEnd_  = false;
	angles_ [0] = 0.0f;
	angles_ [1] = 0.0f;
	angles_ [2] = 0.0f;
}

void Controller::handleKey ( char key, bool isPressed )
{
}

void Controller::handleKeyboard ( const Keyboard& keyboard )
{
	const bool* keys = keyboard.getKeys ();

	if ( keys [VK_ESCAPE] ) {
		shouldEnd_ = true;
		return;
	}

	if ( keys ['w'] || keys ['W'] )
		camera_ -> move ( vector3D ( 0.0f, 0.0f, 1.0f  ) );

	if ( keys ['s'] || keys ['S'] )
		camera_ -> move ( vector3D ( 0.0f, 0.0f, -1.0f  ) );

	if ( keys ['a'] || keys ['A'] )
		camera_ -> move ( vector3D ( -1.0f, 0.0f, 0.0f  ) );

	if ( keys ['d'] || keys ['D'] )
		camera_ -> move ( vector3D ( 1.0f, 0.0f, 0.0f  ) );
}

void Controller::handleMouse ( const Mouse& mouse )
{
	mouse.getMoveAngles ( angles_ );

	camera_ -> setEulerAngles ( angles_ [1], angles_ [0], angles_ [2] );
}

int Controller::update ()
{
	if ( shouldEnd_ )
		return CTRL_QUIT;

	view_  -> lock   ();
	model_ -> render ( camera_ );
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
	font_ -> print ( "fps = %i", (int)fps );
	glPopMatrix    ();
}
