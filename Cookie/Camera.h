#ifndef __CAMERA__
#define __CAMERA__

#include <vector3D.h>
#include <matrix3D.h>

class Camera
{
public:
	Camera ();
	Camera ( const Camera& );

	const vector3D& getPosition      () const;
	const vector3D& getViewDirection () const;
	const vector3D& getUpDirection   () const;
	const vector3D& getSideDirection () const;

	void setPosition ( const vector3D& );

	void move ( const vector3D& );

	vector3D mapToWorld   ( const vector3D& ) const;
	vector3D mapFromWorld ( const vector3D& ) const;
	//vector3D mapToScreen  ( const vector3D& ) const;

	void setEulerAngles ( float yaw, float pitch, float roll );

	void buildHomogeneousMatrix ( float matrix [16] ) const;

private:
	vector3D position_;
	vector3D viewDir_;
	vector3D upDir_;
	vector3D sideDir_;

	void update (); // Compute vectors.
};

#endif