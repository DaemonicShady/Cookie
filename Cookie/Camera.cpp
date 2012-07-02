#include "Camera.h"

Camera::Camera ()
{
	position_ = vector3D ( 0.0f, 0.0f, 0.0f );
	viewDir_  = vector3D ( 0.0f, 0.0f, 1.0f );
	upDir_    = vector3D ( 0.0f, 1.0f, 0.0f );
	sideDir_  = vector3D ( 1.0f, 0.0f, 0.0f );
}

Camera::Camera ( const Camera& camera )
{
	position_ = camera.position_;
	viewDir_  = camera.viewDir_;
	upDir_    = camera.upDir_;
	sideDir_  = camera.sideDir_;
}

void Camera::setPosition ( const vector3D& position )
{
	position_ = position;
}

const vector3D& Camera::getPosition () const
{
	return position_;
}

void Camera::move ( const vector3D& v )
{
	position_ = mapToWorld ( v );
}

vector3D Camera::mapToWorld ( const vector3D& v ) const
{
	return position_ + v.x * sideDir_ + v.y * upDir_ + v.z * viewDir_;
}

vector3D Camera::mapFromWorld ( const vector3D& v ) const
{
	vector3D tmp ( position_ - v );

	return vector3D ( tmp & sideDir_, tmp & upDir_, tmp & viewDir_ );
}

void Camera::setEulerAngles ( float yaw, float pitch, float roll )
{
	viewDir_ = vector3D ( 0.0f, 0.0f, 1.0f );
	upDir_   = vector3D ( 0.0f, 1.0f, 0.0f );
	sideDir_ = vector3D ( 1.0f, 0.0f, 0.0f );

	matrix3D rotate ( matrix3D::getYawPitchRollMatrix ( yaw, pitch, roll ) );

	viewDir_ = rotate * viewDir_;
	upDir_   = rotate * upDir_;
	sideDir_ = rotate * sideDir_;

//	update ();
}

void Camera::buildHomogeneousMatrix ( float matrix [16] ) const
{
	matrix [ 0] = sideDir_.x;
	matrix [ 1] = upDir_.x;
	matrix [ 2] = -viewDir_.x;
	matrix [ 3] = 0.0f;
	
	matrix [ 4] = sideDir_.y;
	matrix [ 5] = upDir_.y;
	matrix [ 6] = -viewDir_.y;
	matrix [ 7] = 0.0f;
	
	matrix [ 8] = sideDir_.z;
	matrix [ 9] = upDir_.z;
	matrix [10] = -viewDir_.z;
	matrix [11] = 0.0f;

	matrix [12] = 0.0f;
	matrix [13] = 0.0f;
	matrix [14] = 0.0f;
	matrix [15] = 1.0f;
}

void Camera::update ()
{
	viewDir_.normalize ();

	upDir_  -= ( upDir_ & viewDir_ ) * viewDir_;

	sideDir_ = upDir_ ^ viewDir_;

	upDir_.normalize   ();
	sideDir_.normalize ();
}
