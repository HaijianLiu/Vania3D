
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Quaternion::Quaternion() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Quaternion::~Quaternion() {

}


/*------------------------------------------------------------------------------
< Constructor from values >
------------------------------------------------------------------------------*/
Quaternion::Quaternion(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


/*------------------------------------------------------------------------------
< Constructor from Assimp quaternion >
------------------------------------------------------------------------------*/
Quaternion::Quaternion(const aiQuaternion& assimpQuaternion) {
	this->x = assimpQuaternion.x;
	this->y = assimpQuaternion.y;
	this->z = assimpQuaternion.z;
	this->w = assimpQuaternion.w;
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


aiQuaternion Quaternion::getAissmp() const {
	aiQuaternion aiquaternion;
	aiquaternion.x = this->x;
	aiquaternion.y = this->y;
	aiquaternion.z = this->z;
	aiquaternion.w = this->w;
	return aiquaternion;
}


/*------------------------------------------------------------------------------
< operator* >
------------------------------------------------------------------------------*/
Quaternion Quaternion::operator*(const Quaternion& right) const {
	const float w = (this->w * right.w) - (this->x * right.x) - (this->y * right.y) - (this->z * right.z);
	const float x = (this->x * right.w) + (this->w * right.x) + (this->y * right.z) - (this->z * right.y);
	const float y = (this->y * right.w) + (this->w * right.y) + (this->z * right.x) - (this->x * right.z);
	const float z = (this->z * right.w) + (this->w * right.z) + (this->x * right.y) - (this->y * right.x);

	Quaternion result = Quaternion(x, y, z, w);
	return result;
}

Quaternion Quaternion::operator*(const glm::vec3& vector) const {
	const float w = - (this->x * vector.x) - (this->y * vector.y) - (this->z * vector.z);
	const float x =   (this->w * vector.x) + (this->y * vector.z) - (this->z * vector.y);
	const float y =   (this->w * vector.y) + (this->z * vector.x) - (this->x * vector.z);
	const float z =   (this->w * vector.z) + (this->x * vector.y) - (this->y * vector.x);

	Quaternion result = Quaternion(x, y, z, w);
	return result;
}
