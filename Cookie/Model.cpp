#include <Windows.h>
#include <gl\gl.h>

#include "Model.h"
#include "View.h"
#include "Camera.h"

Model::Model ( View* view )
{
	view_   = view;
}

Model::~Model ()
{
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

	glTranslatef   ( 0.0f, -0.5f, 0.0f );
	
	glColor3f ( 0.0f, 0.0f, 1.0f );
	glBegin ( GL_QUADS );
	glVertex3f ( -1.0f, 0.0f,  0.0f );
	glVertex3f (  1.0f, 0.0f,  0.0f );
	glVertex3f (  1.0f, 0.0f,  3.0f );
	glVertex3f ( -1.0f, 0.0f,  3.0f );
	glEnd ();
}
