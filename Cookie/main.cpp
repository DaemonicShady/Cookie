#include <Windows.h>

#include "Application.h"

#include <iostream>
#include <fstream>

int APIENTRY WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					   LPSTR cmdLine, int cmdShow )
{
	std::ofstream fout ( "log.txt" );
	std::cerr.rdbuf ( fout.rdbuf () );
	std::cerr << "Starting program...\n";

	Application* app = new Application ( "Cookie", hInstance );

	app -> run ();

	delete app;

	fout.close ();
	//std::remove ( "log.txt" );

	return 0;
}
