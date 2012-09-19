#include <Windows.h>
#include <gl\gl.h>
#include <ExtensionGL.h>

#include "VBO.h"

VBO::VBO ()
{
	glGenBuffers ( 1, &id_ );

	ok_     = (glIsBuffer ( id_ ) == GL_TRUE);
	target_ = 0;
}

VBO::~VBO ()
{
	glDeleteBuffers ( 1, &id_ );
}

unsigned VBO::getId () const
{
	return id_;
}

void VBO::bind ( GLenum target )
{
	glBindBuffer ( target_ = target, id_ );
}

void VBO::unbind ()
{
	glBindBuffer ( target_, 0 );
}

void VBO::setData ( unsigned size, const void* data, GLenum usage )
{
	glBufferData ( target_, size, data, usage );
}

void VBO::setSubData ( unsigned offset, unsigned size, const void* data )
{
	glBufferSubData ( target_, offset, size, data );
}

void VBO::getSubData ( unsigned offset, unsigned size, void* data )
{
	glGetBufferSubData ( target_, offset, size, data );
}

void* VBO::map ( GLenum access )
{
	return glMapBuffer ( target_, access );
}

bool VBO::unmap ()
{
	return glUnmapBuffer ( target_ ) == GL_TRUE;
}

void VBO::clear ()
{
	glBufferData ( target_, 0, 0, 0 );
}

