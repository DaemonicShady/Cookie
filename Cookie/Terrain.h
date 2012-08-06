#ifndef __TERRAIN__
#define __TERRAIN__

class  Texture;
class  VAO;
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

	float getHeight ( float x, float z ) const;
	float getLengthX () const;
	float getLengthZ () const;

	void setScale ( float scale, float heightScale );

private:
	int     format_;
	float   scale_;
	float   heightScale_;
	Vertex* vertices_;
	float** heights_;
	int     nVertices_;
	int     nX_;
	int     nZ_;

	unsigned int vertexBufferId_;

	VAO*    vao_;

	void     computeNormals ();
	vector3D computeNormal  ( const vector3D& A, const vector3D& B,
		                      const vector3D& C ) const;
};

#endif