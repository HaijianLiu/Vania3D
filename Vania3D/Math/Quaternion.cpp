
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Quaternion::Quaternion(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Quaternion::~Quaternion() {

}


/*------------------------------------------------------------------------------
< normalize >
------------------------------------------------------------------------------*/
void Quaternion::normalize() {
	float Length = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
	this->x /= Length;
	this->y /= Length;
	this->z /= Length;
	this->w /= Length;
}


/*------------------------------------------------------------------------------
< degrees >
------------------------------------------------------------------------------*/
glm::vec3 Quaternion::degrees() {
	float f[3];

	f[0] = atan2(x * z + y * w, x * w - y * z);
	f[1] = acos(-x * x - y * y - z * z - w * w);
	f[2] = atan2(x * z - y * w, x * w + y * z);

	f[0] = glm::degrees(f[0]);
	f[1] = glm::degrees(f[1]);
	f[2] = glm::degrees(f[2]);

	return glm::vec3(f[0], f[1], f[2]);
}


/*------------------------------------------------------------------------------
< operator* >
------------------------------------------------------------------------------*/
Quaternion operator*(const Quaternion& l, const Quaternion& r) {
	const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
	const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
	const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
	const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

	Quaternion ret(x, y, z, w);

	return ret;
}

Quaternion operator*(const Quaternion& q, const glm::vec3& v) {
	const float w = - (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
	const float x =   (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
	const float y =   (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
	const float z =   (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

	Quaternion ret(x, y, z, w);

	return ret;
}
