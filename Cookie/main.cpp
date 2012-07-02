#include <Windows.h>

#include "Application.h"

#include <fstream>

int APIENTRY WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					   LPSTR cmdLine, int cmdShow )
{
	std::ofstream fout ( "log.txt" );
	fout << "Starting program...\n";
	fout.close();

	Application* app = new Application ( "Cookie", hInstance );

	app -> run ();

	delete app;

	std::remove ( "log.txt" );

	return 0;
}
