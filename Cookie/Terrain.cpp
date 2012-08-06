#include <iostream>

#include <windows.h>
#include <gl\gl.h>

#include <ExtensionGL.h>
#include <TextureGL.h>
#include <Texture.h>
#include <vector2D.h>
#include <vector3D.h>

#include "Terrain.h"
#include "VAO.h"

using std::cerr;

#define T2_C4_N3_V3 0
#define T2_N3_V3    1
#define C4_N3_V3    2
#define N3_V3       3

struct Vertex
{
	float    texel [2];
	byte     color [4];
	vector3D normal;
	vector3D coord;
};

Terrain::Terrain ( const char* heightmap, const char* colormap,
	               const char* texture, float scale, float heightScale )
{
	Texture* heightTexture = getTexture ( heightmap );

	if ( heightTexture == 0 ) {
		cerr << "Error loading texture """ << heightmap << """\n";
		return;
	}

	Texture* colorTexture = getTexture ( colormap );
	Texture* realTexture  = 0;//getTexture ( texture );

	if ( texture != 0 )
		format_ = ( colormap != 0 ) ? T2_C4_N3_V3 : T2_N3_V3;
	else
		format_ = ( colormap != 0 ) ? C4_N3_V3 : N3_V3;

	scale_       = scale;
	heightScale_ = heightScale;

	nX_ = heightTexture -> getWidth ();
	nZ_ = heightTexture -> getHeight ();
	int nQuads = (nX_ - 1) * (nZ_ - 1);
	nVertices_ = 6 * (nX_ - 1) * (nZ_ - 1);

	vertices_ = new Vertex [nVertices_];

	heights_          = loadHeightmap ( heightTexture );
	byte*   colorData = colorTexture -> getData ();

	for ( int i = 0; i < nX_; i++ )
		for ( int j = 0; j < nZ_; j++ )
			heights_ [i][j] *= heightScale_;

	int   iVertex = 0;
	int   iData   = 0;
	float x, z;

	for ( int i = 0; i < nX_ - 1; i++ )
		for ( int j = 0; j < nZ_ - 1; j++ ) {
			x = i * scale_; z = j * scale_;
			vertices_ [iVertex++].coord = vector3D ( x, heights_ [i][j], z );
			vertices_ [iVertex++].coord = vector3D ( x + scale_, heights_ [i + 1][j], z );
			vertices_ [iVertex++].coord = vector3D ( x + scale_, heights_ [i + 1][j + 1], z + scale_ );
			vertices_ [iVertex++].coord = vector3D ( x, heights_ [i][j], z );
			vertices_ [iVertex++].coord = vector3D ( x + scale_, heights_ [i + 1][j + 1], z + scale_ );
			vertices_ [iVertex++].coord = vector3D ( x, heights_ [i][j + 1], z + scale_ );
		}

	iVertex = 0;
	for ( int i = 0; i < nX_ - 1; i++ )
		for ( int j = 0; j < nZ_ - 1; j++ ) {
			iData = 3 * (j * nX_ + i);
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData] );
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData + 3] );
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData + 3 + 3 * nX_] );
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData] );
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData + 3 + 3 * nX_] );
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData + 3 * nX_] );
		}
	/***************************************/

	delete heightTexture;
	delete colorTexture;
	delete realTexture;

	computeNormals ();
	uploadToGpu    ();
}

Terrain::Terrain ( Texture* heightmap, Texture* colormap, Texture* texture,
	               float scale, float heightScale )
{
	// Unused.
	if ( texture != 0 )
		format_ = ( colormap != 0 ) ? T2_C4_N3_V3 : T2_N3_V3;
	else
		format_ = ( colormap != 0 ) ? C4_N3_V3 : N3_V3;

	scale_       = scale;
	heightScale_ = heightScale;

	int width  = heightmap -> getWidth ();
	int height = heightmap -> getHeight ();
	nVertices_ = 6 * (width - 1) * (height - 1);

	vertices_ = new Vertex [6 * nVertices_];

	byte* heightData = heightmap -> getData ();
	byte* colorData  = colormap  -> getData ();

	int iVertex = 0;
	int iData   = 0;

	for ( int i = 0; i < width; i++ )
		for ( int j = 0; j < height; j++ ) {
			iData = j * width + i;
			vertices_ [iVertex++].coord = vector3D ( i * scale_, heightData [iData] * heightScale_, j * scale_ );
			vertices_ [iVertex++].coord = vector3D ( (i + 1) * scale_, heightData [iData + 1] * heightScale_, j * scale_ );
			vertices_ [iVertex++].coord = vector3D ( (i + 1) * scale_, heightData [iData + width + 1] * heightScale_, (j + 1) * scale_ );
			vertices_ [iVertex++].coord = vector3D ( i * scale_, heightData [iData] * heightScale_, j * scale_ );
			vertices_ [iVertex++].coord = vector3D ( (i + 1) * scale_, heightData [iData + width + 1] * heightScale_, (j + 1) * scale_ );
			vertices_ [iVertex++].coord = vector3D ( i * scale_, heightData [iData + width] * heightScale_, (j + 1) * scale_ );
		}

	iVertex = 0;
	for ( int i = 0; i < width; i++ )
		for ( int j = 0; j < height; j++ ) {
			iData = 3 * (j * width + i);
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData] );
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData + 3] );
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData + 3 + 3 * width] );
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData] );
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData + 3 + 3 * width] );
			*(dword *)vertices_ [iVertex++].color = makeRGBA ( &colorData [iData + 3 * width] );
		}
}

Terrain::~Terrain ()
{
	delete [] vertices_;
	
	for ( int i = 0; i < nZ_; i++ )
		delete [] heights_ [i];
	delete heights_;

	removeFromGpu ();
}

float Terrain::getLengthX () const
{
	return nX_ * scale_;
}

float Terrain::getLengthZ () const
{
	return nZ_ * scale_;
}

void Terrain::uploadToGpu ()
{
	initializeExtensions ();
	glGenBuffers ( 1, &vertexBufferId_ );
	glBindBuffer ( GL_ARRAY_BUFFER, vertexBufferId_ );
	glBufferData ( GL_ARRAY_BUFFER, sizeof(Vertex) * nVertices_,
		           vertices_, GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );


	int colorOffs  = (int)&vertices_ [0].color  - (int)&vertices_ [0];
	int normalOffs = (int)&vertices_ [0].normal - (int)&vertices_ [0];
	int coordOffs  = (int)&vertices_ [0].coord  - (int)&vertices_ [0];

	vao_ = new VAO ();

	vao_ -> bind ();
	glPushClientAttrib  ( GL_CLIENT_VERTEX_ARRAY_BIT );
	glEnableClientState ( GL_COLOR_ARRAY );
	glEnableClientState ( GL_NORMAL_ARRAY );
	glEnableClientState ( GL_VERTEX_ARRAY );

	glBindBuffer      ( GL_ARRAY_BUFFER, vertexBufferId_ );
	glColorPointer    ( 4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void *)colorOffs );
	glNormalPointer   ( GL_FLOAT, sizeof(Vertex), (void *)normalOffs );
	glVertexPointer   ( 3, GL_FLOAT, sizeof(Vertex), (void *)coordOffs );
	vao_ -> unbind ();
}

void Terrain::removeFromGpu ()
{
	glBindBuffer    ( GL_ARRAY_BUFFER, vertexBufferId_ );
	glBufferData    ( GL_ARRAY_BUFFER, 0, 0, 0 );
	glDeleteBuffers ( 1, &vertexBufferId_ );
}

void Terrain::render ()
{
	vao_ -> bind ();
	glDrawArrays ( GL_TRIANGLES, 0, nVertices_ );
	vao_ -> unbind ();
}

float Terrain::getHeight ( float x, float z ) const
{
	float lengthX = nX_ * scale_;
	float lengthZ = nZ_ * scale_;

	if ( x < 0.0f || z < 0.0f || x > lengthX || z > lengthZ )
		return 0.0f;

	int   i  = int(x / scale_);
	int   j  = int(z / scale_);
	float x0 = i * scale_;
	float z0 = j * scale_;

	vector3D A ( x0, heights_ [i][j], z0 );
	vector3D B ( x0 + scale_, heights_ [i + 1][j + 1], z0 + scale_ );
	vector3D C = ( z - z0 > x - x0 ) ?
		vector3D ( x0, heights_ [i][j + 1], z0 + scale_ ) :
	    vector3D ( x0 + scale_, heights_ [i + 1][j], z0 );

	vector3D surface = computeNormal ( A, B, C );
	float d = - surface & A;

	return -(surface.x * x + surface.z * z + d) / surface.y;
}

void Terrain::computeNormals ()
{
	vector3D normal;

	for ( int i = 0; i < nVertices_; i += 3 ) {
		normal = -computeNormal ( vertices_ [i].coord, vertices_ [i + 1].coord,
			                      vertices_ [i + 2].coord ).normalize ();
		vertices_ [i    ].normal = normal;
		vertices_ [i + 1].normal = normal;
		vertices_ [i + 2].normal = normal;
	}
}

vector3D Terrain::computeNormal ( const vector3D& A, const vector3D& B,
	                              const vector3D& C ) const
{
	return vector3D(A.y * (B.z - C.z) + B.y * (C.z - A.z) + C.y * (A.z - B.z),
		            A.z * (B.x - C.x) + B.z * (C.x - A.x) + C.z * (A.x - B.x),
					A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
}

	//int colorOffset  = (int)&vertices_ [0].color  - (int)&vertices_ [0];
	//int vertexOffset = (int)&vertices_ [0].vertex - (int)&vertices_ [0];
	//glPolygonMode ( GL_FRONT, GL_LINE );
	/*glColor3f ( 0.0f, 1.0f, 0.0f );
	glEnableClientState  ( GL_COLOR_ARRAY );
	glEnableClientState  ( GL_NORMAL_ARRAY );
	glEnableClientState  ( GL_VERTEX_ARRAY );
	glColorPointer       ( 4, GL_UNSIGNED_BYTE, sizeof(Vertex), vertices_ [0].color );
	glNormalPointer      ( GL_FLOAT, sizeof(Vertex), vertices_ [0].normal );
	glVertexPointer      ( 3, GL_FLOAT, sizeof(Vertex), vertices_ [0].coord );
	glDrawArrays         ( GL_TRIANGLES, 0, nVertices_ );
	glDisableClientState ( GL_VERTEX_ARRAY );
	glDisableClientState ( GL_NORMAL_ARRAY );
	glDisableClientState ( GL_COLOR_ARRAY );*/
