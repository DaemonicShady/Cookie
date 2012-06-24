#include <Windows.h>

#include "Application.h"

int APIENTRY WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					   LPSTR cmdLine, int cmdShow )
{
	Application* app = new Application ( "Cookie", hInstance );

	app -> run ();

	delete app;

	return 0;
}
