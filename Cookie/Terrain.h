#ifndef __TERRAIN__
#define __TERRAIN__

class  Texture;
struct Vertex;

class Terrain
{
public:
	Terrain ( const char* hightmap, const char* colormap, const char* texture,
		      float scale = 1.0f, float heightScale = 1.0f );
	Terrain ( Texture* hightmap, Texture* colormap, Texture* texture,
		      float scale = 1.0f, float heightScale = 1.0f );
	~Terrain ();

	void uploadToGpu   ();
	void removeFromGpu ();
	void render ();

	void setScale ( float scale, float heightScale );

private:
	int     format_;
	float   scale_;
	float   heightScale_;
	Vertex* vertices_;
	int     nVertices_;

//	float*  computeNormals ( float* heightmap = 0 ); // Unused till now.
};

#endif