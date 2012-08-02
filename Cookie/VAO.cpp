#include <ExtensionGL.h>

#include "VAO.h"

VAO::VAO ()
{
	glGenVertexArrays ( 1, &id_ );
}

VAO::~VAO ()
{
	glDeleteVertexArrays ( 1, &id_ );
}

bool VAO::isOk () const
{
	return glIsVertexArray ( id_ ) == GL_TRUE;
}

void VAO::bind ()
{
	glBindVertexArray ( id_ );
}

void VAO::unbind ()
{
	glBindVertexArray ( 0 );
}
