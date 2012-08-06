#ifndef __MODEL__
#define __MODEL__

class View;
class Controller;
class Camera;
class Terrain;
class Shader;
class Grass;

class Model
{
public:
	Model  ( View* view_ );
	~Model ();

	void    render ( Camera* );

private:
	View*       view_;
	Controller* controller_;

	Terrain*    terrain_;
	Grass*      grass_;
	Shader*     shader_;
};

#endif