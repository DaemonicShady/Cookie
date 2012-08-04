#include <Windows.h>
#include <iostream>
#include <gl\gl.h>
#include <Shader.h>
#include <vector4D.h>

#include "Model.h"
#include "View.h"
#include "Camera.h"

#include "Terrain.h"

using std::cerr;

const vector4D diffuseLight  = ( 1.0f, 1.0f, 1.0f, 1.0f );
const vector4D ambientLight  = ( 0.0f, 0.0f, 0.0f, 0.0f );
const vector4D lightPosition = ( 0.0f, 1.0f, 0.0f, 0.0f );

Model::Model ( View* view )
{
	view_    = view;
	terrain_ = new Terrain ( "textures/heightmap.bmp",
		                     "textures/colormap.bmp", 0, 5.0f, 1.0f / 16.0f );

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
	glTranslatef ( 0.0f, -0.5f, 0.0f );

	//glLightf ( GL_LIGHT0,
//	glEnable ( GL_CULL_FACE );
//	glColorMaterial ( GL_FRONT, GL_DIFFUSE );
//	glColorMaterial ( GL_BACK, GL_DIFFUSE );
//	glEnable ( GL_COLOR_MATERIAL );

//	glLightModeli ( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

//	glLightfv ( GL_LIGHT0, GL_POSITION, lightPosition );
//	glLightfv ( GL_LIGHT0, GL_DIFFUSE,  diffuseLight  );
//	glLightfv ( GL_LIGHT0, GL_AMBIENT,  ambientLight  );

	glEnable ( GL_DEPTH_TEST );
//	glEnable ( GL_LIGHTING );
//	glEnable ( GL_LIGHT0 );

	shader_  -> bind ();
	terrain_ -> render ();
	shader_  -> unbind ();

/*	glColor3f  ( 1.0f, 0.0f, 0.0f );
	glNormal3f ( 0.0f, 1.0f, 0.0f );
	glBegin ( GL_QUADS );
	glVertex3f ( 0.0f, 0.0f, 0.0f );
	glVertex3f (  1.0f, 0.0f, 0.0f );
	glVertex3f (  1.0f, 0.0f, 3.0f );
	glVertex3f ( 0.0f, 0.0f, 3.0f );
	glEnd   ();*/

//	glDisable ( GL_LIGHT0 );
//	glDisable ( GL_LIGHTING );
	glDisable ( GL_DEPTH_TEST );
}
