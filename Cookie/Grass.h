#ifndef __GRASS__
#define __GRASS__

#include <list>

struct GrassObject;
class  Terrain;
class  Shader;
class  Camera;

using std::list;

class Grass
{
public:
	Grass  ( Terrain*, const char* textureFile );
	~Grass ();

	void render ( Camera* );

private:
	list<GrassObject* > grass_;
	Shader*             shader_;
	unsigned int        texture_;
};

#endif