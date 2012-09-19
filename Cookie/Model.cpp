#include <Windows.h>
#include <iostream>
#include <gl\gl.h>
#include <Shader.h>
#include <vector4D.h>

#include "Model.h"
#include "View.h"
#include "Camera.h"

#include "Terrain.h"
#include "Grass.h"

using std::cerr;

const vector4D diffuseLight  = ( 1.0f, 1.0f, 1.0f, 1.0f );
const vector4D ambientLight  = ( 0.0f, 0.0f, 0.0f, 0.0f );
const vector4D lightPosition = ( 0.0f, 1.0f, 0.0f, 0.0f );

Model::Model ( View* view )
{
	view_    = view;
	terrain_ = new Terrain ( "textures/heightmap.bmp",
		                     "textures/colormap.bmp", 0, 5.0f, 1.0f / 16.0f );

	grass_   = new Grass ( terrain_, "textures/grass_new2.bmp" );

	shader_  = new Shader ();
	if ( !shader_ -> isSupported () )
		cerr << "Shader model not supported!\n";

	shader_ -> loadShaders ( "shaders/blinn.vs", "shaders/blinn.fs" );
	cerr << shader_ -> getLog ();

	shader_ -> bind ();
	shader_ -> setUniformVector ( "lightpos", lightPosition );
	shader_ -> setUniformVector ( "eyePos", vector4D ( 0, 0, 0, 1 ) );
	shader_ -> unbind ();
}

Model::~Model ()
{
	delete shader_;
	delete grass_;
	delete terrain_;
}

void Model::render ( Camera* camera )
{
	glMatrixMode   ( GL_MODELVIEW );
	glLoadIdentity ();

	float m [16];
	vector3D v;

	camera -> buildHomogeneousMatrix ( m );
	v = camera -> getPosition ();

	glMultMatrixf ( m );
	glTranslatef ( -v.x, -v.y, -v.z );
	glTranslatef ( 0.0f, -4.0f, 0.0f );

	glEnable ( GL_DEPTH_TEST );

	shader_  -> bind ();
	terrain_ -> render ();
	shader_  -> unbind ();

	glDisable ( GL_DEPTH_TEST );

	grass_ -> render ( camera );
}
