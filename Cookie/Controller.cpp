#include <Windows.h>

#include "Controller.h"
#include "Model.h"
#include "View.h"

Controller::Controller ( Model* model, View* view )
{
	view_  = view;
	model_ = model;
	view_ -> setController ( this );

	for ( int i = 0; i < 256; i++ )
		keys [i] = false;
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

	return CTRL_CONTINUE;
}

void Controller::draw ()
{
	view_  -> lock   ();
	model_ -> render ();
	view_  -> unlock ();
}
