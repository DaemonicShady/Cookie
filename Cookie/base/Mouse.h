#ifndef __MOUSE__
#define __MOUSE__

#define LEFTBUTTON     1
#define RIGHTBUTTON    2
#define WHEEL          4

class Mouse
{
public:
	Mouse ( int mouseX, int mouseY );

	void setState      ( int x, int y, bool leftButtonDown,
		                 bool rightButtonDown, bool wheelDown );
	void setPosition   ( int mouseX, int mouseY );
	void changeButton  ( int iButton );

	void getMoveAngles ( float angles [3] ) const;

	bool isPressed     ( int iButton ) const;

private:
	int  mouseFlags_;

	int  mouseX_;
	int  mouseY_;
};

#endif