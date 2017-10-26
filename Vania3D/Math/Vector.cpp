
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Vector3::Vector3() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Vector3::~Vector3() {

}


Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}


/*------------------------------------------------------------------------------
< Constructor from Assimp matrix >
------------------------------------------------------------------------------*/
Vector3::Vector3(const aiVector3D& assimpVector) {
	this->x = assimpVector.x;
	this->y = assimpVector.y;
	this->z = assimpVector.z;
}

Vector3 Vector3::operator+(const Vector3& v) const {
	Vector3 result = Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
	return result;
}

Vector3 Vector3::operator-(const Vector3& v) const {
	Vector3 result = Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
	return result;
}

Vector3 operator*(const float& f, const Vector3& v) {
	Vector3 result = Vector3(v.x * f, v.y * f, v.z * f);
	return result;
}
