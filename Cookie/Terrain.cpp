#include <iostream>

#include <windows.h>
#include <gl\gl.h>

#include <TextureGL.h>
#include <Texture.h>
#include <vector2D.h>
#include <vector3D.h>

#include "Terrain.h"

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

//	Terrain ( heightTexture, colorTexture, realTexture );
	/***************************************/
	if ( texture != 0 )
		format_ = ( colormap != 0 ) ? T2_C4_N3_V3 : T2_N3_V3;
	else
		format_ = ( colormap != 0 ) ? C4_N3_V3 : N3_V3;

	scale_       = scale;
	heightScale_ = heightScale;

	int width  = heightTexture -> getWidth ();
	int height = heightTexture -> getHeight ();
	nVertices_ = 6 * (width - 1) * (height - 1);

	vertices_ = new Vertex [6 * nVertices_];

	byte* heightData = getGrayscaleData ( heightTexture );
	byte* colorData  = colorTexture -> getData ();

	int iVertex = 0;
	int iData   = 0;

	for ( int i = 0; i < width - 1; i++ )
		for ( int j = 0; j < height - 1; j++ ) {
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
	/***************************************/

	delete heightTexture;
	delete colorTexture;
	delete realTexture;
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
}

void Terrain::uploadToGpu ()
{
}

void Terrain::removeFromGpu ()
{
}

void Terrain::render ()
{
	//int colorOffset  = (int)&vertices_ [0].color  - (int)&vertices_ [0];
	//int vertexOffset = (int)&vertices_ [0].vertex - (int)&vertices_ [0];
	//glPolygonMode ( GL_FRONT, GL_LINE );
	glColor3f ( 0.0f, 1.0f, 0.0f );
	glEnableClientState ( GL_COLOR_ARRAY );
	glEnableClientState ( GL_VERTEX_ARRAY );
	glColorPointer       ( 4, GL_UNSIGNED_BYTE, sizeof(Vertex), vertices_ [0].color );
	glVertexPointer      ( 3, GL_FLOAT, sizeof(Vertex), vertices_ [0].coord );
	glDrawArrays         ( GL_TRIANGLES, 0, nVertices_ );
	glDisableClientState ( GL_VERTEX_ARRAY );
	glDisableClientState ( GL_COLOR_ARRAY );
}
