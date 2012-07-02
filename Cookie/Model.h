#ifndef __MODEL__
#define __MODEL__

class View;
class Controller;
class Camera;

class Model
{
public:
	Model  ( View* view_ );
	~Model ();

	void    render ( Camera* );

private:
	View*       view_;
	Controller* controller_;
};

#endif