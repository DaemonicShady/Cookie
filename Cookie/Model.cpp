#include <Windows.h>
#include <gl\gl.h>

#include "Model.h"
#include "View.h"
#include "Camera.h"

#include "Terrain.h"

Model::Model ( View* view )
{
	view_    = view;
	terrain_ = new Terrain ( "textures/heightmap.bmp",
		                     "textures/colormap.bmp", 0, 5.0f );
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

	glEnable ( GL_DEPTH_TEST );

	terrain_ -> render ();

	glDisable ( GL_DEPTH_TEST );
	
	/*glColor3f ( 0.0f, 0.0f, 1.0f );
	glBegin ( GL_QUADS );
	glVertex3f ( -1.0f, 0.0f,  0.0f );
	glVertex3f (  1.0f, 0.0f,  0.0f );
	glVertex3f (  1.0f, 0.0f,  3.0f );
	glVertex3f ( -1.0f, 0.0f,  3.0f );
	glEnd ();*/
}
