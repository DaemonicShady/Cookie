#ifndef __CONTROLLER__
#define __CONTROLLER__

#define CTRL_QUIT     0
#define CTRL_CONTINUE 1

class View;
class Model;
class Fraps;
class Font;
class Keyboard;
class Mouse;
class Camera;

class Controller
{
public:
	Controller ( Model* model, View* view );
	~Controller () {}

	void    handleKeyboard ( const Keyboard& );
	void    handleMouse    ( const Mouse& );
	void    handleKey      ( char key, bool isPressed );

	int		update (); //!< Main function that works in cycle.
	void	draw (); //!< Redraws view using model if there are some changes.

private:
	Model*  model_;
	View*   view_;
	Camera* camera_;

	Fraps*  fraps_;
	Font*   font_;

	bool    shouldEnd_;
	float   angles_ [3];
};

#endif
