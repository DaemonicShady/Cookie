#ifndef __MODEL__
#define __MODEL__

class View;
class Controller;
class Font;

class Model
{
public:
	Model  ( View* view_ );
	~Model ();

	void render ();

private:
	View*       view_;
	Controller* controller_;

	Font* font_;
};

#endif