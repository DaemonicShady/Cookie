#ifndef __KEYBOARD__
#define __KEYBOARD__

class Keyboard
{
public:
	Keyboard ();

	void  setKey    ( char key, bool isPressed );
	bool  isPressed ( char key ) const;

	const bool* getKeys () const;

private:
	bool  keys_ [256];
};

#endif