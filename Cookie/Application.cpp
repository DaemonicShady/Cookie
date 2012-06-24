#include <Windows.h>

#include "Application.h"
#include "Model.h"
#include "OpenGlView.h"
#include "Controller.h"

Application::Application ( string appName, HINSTANCE hInstance )
{
	appName_    = appName;
	hInstance_  = hInstance;
	view_       = new OpenGlView ( appName, hInstance, 800, 600, 32 );
	model_      = new Model ();
	controller_ = new Controller ( model_, view_ );
}

Application::~Application ()
{
	delete view_;
	delete model_;
	delete controller_;
}

bool Application::isOk () const
{
	return true;
}

int Application::run ()
{
	bool done = false;
	MSG  msg;

	while ( !done )
	{
		if ( PeekMessage ( &msg, 0, 0, 0, PM_REMOVE ) ) {
			if ( msg.message == WM_QUIT )
				return (int)msg.wParam;
			
			TranslateMessage ( &msg );
			DispatchMessage	 ( &msg );
		}
		else {
			if ( controller_ -> update () == CTRL_QUIT )
				done = true;
		}
	}

	return 0;
}

void Application::setModel ( Model* model )
{
	model_ = model;
}

void Application::setView ( View* view )
{
	view_ = view;
}
