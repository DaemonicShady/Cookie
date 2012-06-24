#ifndef __CONTROLLER__
#define __CONTROLLER__

#define CTRL_QUIT     0
#define CTRL_CONTINUE 1

class View;
class Model;

class Controller
{
public:
	Controller ( Model* model, View* view );
	~Controller () {}

	void	keyboard   ( char key, bool isPressed );
	bool	isPressed  ( char key );
	void	mouseClick ( int param, bool isPressed );

	int		update (); //!< Main function that works in cycle.
	void	draw (); //!< Redraws view using model if there are some changes.

private:
	bool   keys [256];
	Model* model_;
	View*  view_;
};

#endif