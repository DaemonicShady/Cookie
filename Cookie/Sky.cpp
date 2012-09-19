#include <Windows.h>
#include <cmath>
#include <gl\gl.h>

#include <vector3D.h>
#include <ExtensionGL.h>

#include "Sky.h"
#include "VAO.h"

const float PI     = 2 * acos ( 0.0f );
const float PI2    = 2 * PI;
const float halfPI = PI / 2;

struct Vertex
{
	byte     color [4];
	vector3D normal;
	vector3D coord;
};

Sky::Sky ( float radius, float angle, int nBlocksX, int nBlocksY )
{
	float gridX = PI2 / nBlocksX; //!< Angle to the next point in X direction.
	float gridY = PI  / nBlocksY;

	nVertices_  = nBlocksX * (nBlocksY / 2) + 1;
	nTriangles_ = ( nBlocksX * (nBlocksY / 2) ) * 2 + nBlocksX;
	vertices_   = new Vertex [nVertices_];
	indices_    = new int    [nTriangles_ * 3];

	float x, y, z;
	int   i = 0;

	for ( float phi = 0; phi < PI2; phi += gridX ) {
		x = cos (phi) * radius;
		z = sin (phi) * radius;
		for ( float psi = 0; psi < halfPI; psi += gridY ) {
			y = sin (psi) * radius;
			vertices_ [i++].coord = vector3D ( x, y, z );
		}
	}

	int j = 0;

	//!< Can be reorganized to TRIANGLES_STRIP.
	for ( i = 0; i < nTriangles_ - nBlocksX; i++ ) {
		indices_ [j++] = i;
		indices_ [j++] = i + nBlocksX;
		indices_ [j++] = i + 1;
		indices_ [j++] = i + 1;
		indices_ [j++] = i + nBlocksX;
		indices_ [j++] = i + nBlocksX + 1;
	}

	for ( ; i < nTriangles_; i++ ) {
		indices_ [j++] = i;
		indices_ [j++] = nVertices_ - 1;
		indices_ [j++] = i + 1;
	}

	uploadToGpu ();
}

Sky::~Sky ()
{
	delete [] vertices_;
	delete [] indices_;

	removeFromGpu ();
}

void Sky::uploadToGpu ()
{
	initializeExtensions ();
	glGenBuffers ( 1, &vertexBufferId_ );
	glBindBuffer ( GL_ARRAY_BUFFER, vertexBufferId_ );
	glBufferData ( GL_ARRAY_BUFFER, sizeof(Vertex) * nVertices_,
		           vertices_, GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );

	glGenBuffers ( 1, &indexId_ );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexId_ );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * nTriangles_ * 3,
		           indices_, GL_STATIC_DRAW );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

	int coordOffset = (int)&vertices_[0].coord - (int)&vertices_[0];

	vao_ = new VAO ();

	vao_ -> bind ();
	glPushClientAttrib  ( GL_CLIENT_VERTEX_ARRAY_BIT );
	glEnableClientState ( GL_VERTEX_ARRAY );

	glBindBuffer ( GL_ARRAY_BUFFER, vertexBufferId_ );
	glVertexPointer ( 3, GL_FLOAT, sizeof(Vertex), (void *)coordOffset );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexId_ );
	vao_ -> unbind ();
}

void Sky::removeFromGpu ()
{
	glBindBuffer ( GL_ARRAY_BUFFER, vertexBufferId_ );
	glBufferData ( GL_ARRAY_BUFFER, 0, 0, 0 );
	glDeleteBuffers ( 1, &vertexBufferId_ );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, indexId_ );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, 0, 0, 0 );
	glDeleteBuffers ( 1, &indexId_ );
}

void Sky::render ()
{
	vao_ -> bind ();
	glDrawElements ( GL_TRIANGLES, nTriangles_, GL_UNSIGNED_INT, 0 );
	vao_ -> unbind ();
}
