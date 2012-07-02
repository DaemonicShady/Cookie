#include "Keyboard.h"

Keyboard::Keyboard ()
{
	for ( int i = 0; i < 256; i++ )
		keys_ [i] = false;
}

void Keyboard::setKey ( char key, bool isPressed )
{
	keys_ [key] = isPressed;
}

bool Keyboard::isPressed ( char key ) const
{
	return keys_ [key];
}

const bool* Keyboard::getKeys () const
{
	return keys_;
}
