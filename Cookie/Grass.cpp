#include <Windows.h>
#include <gl\gl.h>
#include <iostream>

#include <vector3D.h>
#include <Shader.h>
#include <TextureGL.h>

#include "Grass.h"
#include "Terrain.h"
#include "Camera.h"

using std::cerr;

struct GrassObject
{
	vector3D position [4];
	vector3D texel [4];
};

inline float random ()
{
	return (float) rand () / (float) RAND_MAX;
}

inline float random ( float a, float b )
{
	return a + ( b - a ) * random ();
}

Grass::Grass ( Terrain* terrain, const char* textureFile )
{
	float stride = 1.0f;
	float lengthX = 50.0f;//terrain -> getLengthX ();
	float lengthZ = 50.0f;//terrain -> getLengthZ ();
	int   line    = 0;

	for ( float x0 = 1.0f; x0 < lengthX; x0 += stride, line++ )
		for ( float z0 = 1.0f; z0 < lengthZ; z0 += stride )
		{
			float x = x0 + (random () - 0.5f) * stride;
			float z = z0 + (random () - 0.5f) * stride;

			if ( line & 1 )
				x += 0.5f * stride;

			vector3D point = vector3D ( x, terrain->getHeight(x, z) + 0.5f, z );
			GrassObject* object = new GrassObject;

			object -> position [0] = point;
			object -> position [1] = point;
			object -> position [2] = point;
			object -> position [3] = point;
			object -> texel [0] = vector3D ( 0.0f, 0.0f, 0.0f );
			object -> texel [1] = vector3D ( 1.0f, 0.0f, 0.0f );
			object -> texel [2] = vector3D ( 1.0f, 1.0f, 0.0f );
			object -> texel [3] = vector3D ( 0.0f, 1.0f, 0.0f );

			grass_.push_back ( object );
		}

	cerr << "Created " << grass_.size () << " grass objects.\n";

	texture_ = loadTexture ( textureFile, false );

	shader_ = new Shader;
	shader_ -> loadShaders ( "shaders/grass.vs", "shaders/grass.fs" );
	cerr << shader_ -> getLog ();
}

Grass::~Grass ()
{
	list<GrassObject*>::iterator p;
	for ( p = grass_.begin (); p != grass_.end (); p++ )
		delete *p;

	grass_.clear ();

	glDeleteTextures ( 1, &texture_ );

	delete shader_;
}

void Grass::render ( Camera* camera )
{
	glEnable    ( GL_TEXTURE_2D );
//	glEnable    ( GL_ALPHA_TEST );
//	glAlphaFunc ( GL_GEQUAL, 0.5 );

	//glBindTexture ( GL_TEXTURE_2D, texture_ );

	shader_ -> bind ();
	shader_ -> setUniformVector ( "eyePos", vector3D () );
	shader_ -> setUniformVector ( "up", camera -> getUpDirection () );
	shader_ -> setUniformVector ( "right", camera -> getSideDirection () );
	shader_ -> setTexture ( "grassMap", texture_ );

	list<GrassObject*>::iterator i;
	for ( i = grass_.begin (); i != grass_.end (); i++ ) {
		GrassObject* object = *i;

		glBegin ( GL_QUADS );
		for ( int j = 0; j < 4; j++ ) {
			glTexCoord3fv ( object -> texel [j] );
			glVertex3fv   ( object -> position [j] );
		}
		glEnd ();
	}

	shader_ -> unbind ();

//	glDisable ( GL_ALPHA_TEST );
}
