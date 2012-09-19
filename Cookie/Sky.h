#ifndef __SKY__
#define __SKY__

class  vector3D;
class  VAO;
struct Vertex;

class Sky
{
public:
	Sky  ( float radius, float angle, int nBlocksX, int nBlocksY );
	~Sky ();

	void uploadToGpu   ();
	void removeFromGpu ();
	void render ();
	void initialize ();

private:
	Vertex*  vertices_;
	int*     indices_;
	int      nVertices_;
	int      nTriangles_;
	float    radius_;
	float    height_;
	float    angle_;
	vector3D center_;

	VAO*     vao_;

	unsigned int vertexBufferId_;
	unsigned int indexId_;
};

#endif