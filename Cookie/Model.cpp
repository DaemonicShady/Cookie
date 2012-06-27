#include <Windows.h>
#include <gl\gl.h>
#include <RasterFont.h>

#include "Model.h"
#include "View.h"

Model::Model ( View* view )
{
	view_ = view;
	font_ = new RasterFont ( view -> getHDc (), 24, "Comic Sans MS" );
}

Model::~Model ()
{
}

void Model::render ()
{
	glMatrixMode   ( GL_MODELVIEW );
	glLoadIdentity ();
}
