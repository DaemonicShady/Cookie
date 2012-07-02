#include <Windows.h>

#include "Mouse.h"

Mouse::Mouse ( int mouseX, int mouseY )
{
	mouseX_     = mouseX;
	mouseY_     = mouseY;
	mouseFlags_ = 0;

	SetCursorPos ( mouseX, mouseY );
	ShowCursor   ( false );
}

void Mouse::setState ( int x, int y, bool leftButtonDown,
	                        bool rightButtonDown, bool wheelDown )
{
	mouseX_ = x;
	mouseY_ = y;

	mouseFlags_ = ( leftButtonDown ? 1 : 0 ) | ( rightButtonDown ? 2 : 0 ) |
		          ( wheelDown ? 4 : 0 );
}

void Mouse::changeButton ( int iButton )
{
	mouseFlags_ ^= iButton;
}

void Mouse::setPosition ( int mouseX, int mouseY )
{
	mouseX_     = mouseX;
	mouseY_     = mouseY;

	SetCursorPos ( mouseX, mouseY );
}

bool Mouse::isPressed ( int iButton ) const
{
	return ( mouseFlags_ & iButton ) != 0;
}

void Mouse::getMoveAngles ( float angles [3] ) const
{
	POINT mousePosition;

	GetCursorPos ( &mousePosition );

	if ( mousePosition.x == mouseX_ &&
		 mousePosition.y == mouseY_ )
		 return;

	SetCursorPos ( mouseX_, mouseY_ );

	angles [1] += (float)(mousePosition.x - mouseX_) / 1000.0f;
	angles [0] += (float)(mousePosition.y - mouseY_) / 1000.0f;
}
