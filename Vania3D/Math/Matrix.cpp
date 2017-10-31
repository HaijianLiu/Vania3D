
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Matrix4::Matrix4() {

}

/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Matrix4::~Matrix4() {

}


/*------------------------------------------------------------------------------
< Constructor from values >
------------------------------------------------------------------------------*/
Matrix4::Matrix4(float a00, float a01, float a02, float a03,
	               float a10, float a11, float a12, float a13,
	               float a20, float a21, float a22, float a23,
	               float a30, float a31, float a32, float a33) {
	this->value[0][0] = a00; this->value[0][1] = a01; this->value[0][2] = a02; this->value[0][3] = a03;
	this->value[1][0] = a10; this->value[1][1] = a11; this->value[1][2] = a12; this->value[1][3] = a13;
	this->value[2][0] = a20; this->value[2][1] = a21; this->value[2][2] = a22; this->value[2][3] = a23;
	this->value[3][0] = a30; this->value[3][1] = a31; this->value[3][2] = a32; this->value[3][3] = a33;
}


/*------------------------------------------------------------------------------
< Constructor from Assimp matrix >
------------------------------------------------------------------------------*/
Matrix4::Matrix4(const aiMatrix4x4& assimpMatrix) {
	this->value[0][0] = assimpMatrix.a1; this->value[0][1] = assimpMatrix.a2; this->value[0][2] = assimpMatrix.a3; this->value[0][3] = assimpMatrix.a4;
	this->value[1][0] = assimpMatrix.b1; this->value[1][1] = assimpMatrix.b2; this->value[1][2] = assimpMatrix.b3; this->value[1][3] = assimpMatrix.b4;
	this->value[2][0] = assimpMatrix.c1; this->value[2][1] = assimpMatrix.c2; this->value[2][2] = assimpMatrix.c3; this->value[2][3] = assimpMatrix.c4;
	this->value[3][0] = assimpMatrix.d1; this->value[3][1] = assimpMatrix.d2; this->value[3][2] = assimpMatrix.d3; this->value[3][3] = assimpMatrix.d4;
}

Matrix4::Matrix4(const aiMatrix3x3& assimpMatrix) {
	this->value[0][0] = assimpMatrix.a1; this->value[0][1] = assimpMatrix.a2; this->value[0][2] = assimpMatrix.a3; this->value[0][3] = 0.0f;
	this->value[1][0] = assimpMatrix.b1; this->value[1][1] = assimpMatrix.b2; this->value[1][2] = assimpMatrix.b3; this->value[1][3] = 0.0f;
	this->value[2][0] = assimpMatrix.c1; this->value[2][1] = assimpMatrix.c2; this->value[2][2] = assimpMatrix.c3; this->value[2][3] = 0.0f;
	this->value[3][0] = 0.0f           ; this->value[3][1] = 0.0f           ; this->value[3][2] = 0.0f           ; this->value[3][3] = 1.0f;
}


/*------------------------------------------------------------------------------
< set zero >
------------------------------------------------------------------------------*/
void Matrix4::setZero() {
	memset(this->value, 0, sizeof(this->value));
}


/*------------------------------------------------------------------------------
< set identity >
------------------------------------------------------------------------------*/
void Matrix4::setIdentity() {
	this->value[0][0] = 1.0f; this->value[0][1] = 0.0f; this->value[0][2] = 0.0f; this->value[0][3] = 0.0f;
	this->value[1][0] = 0.0f; this->value[1][1] = 1.0f; this->value[1][2] = 0.0f; this->value[1][3] = 0.0f;
	this->value[2][0] = 0.0f; this->value[2][1] = 0.0f; this->value[2][2] = 1.0f; this->value[2][3] = 0.0f;
	this->value[3][0] = 0.0f; this->value[3][1] = 0.0f; this->value[3][2] = 0.0f; this->value[3][3] = 1.0f;
}


/*------------------------------------------------------------------------------
< set scale transform >
------------------------------------------------------------------------------*/
void Matrix4::setScaleTransform(float scaleX, float scaleY, float scaleZ) {
	this->value[0][0] = scaleX; this->value[0][1] = 0.0f;   this->value[0][2] = 0.0f;   this->value[0][3] = 0.0f;
	this->value[1][0] = 0.0f;   this->value[1][1] = scaleY; this->value[1][2] = 0.0f;   this->value[1][3] = 0.0f;
	this->value[2][0] = 0.0f;   this->value[2][1] = 0.0f;   this->value[2][2] = scaleZ; this->value[2][3] = 0.0f;
	this->value[3][0] = 0.0f;   this->value[3][1] = 0.0f;   this->value[3][2] = 0.0f;   this->value[3][3] = 1.0f;
}


/*------------------------------------------------------------------------------
< set rotate transform >
------------------------------------------------------------------------------*/
void Matrix4::setRotateTransform(float degreeX, float degreeY, float degreeZ) {
	Matrix4 rx, ry, rz;

	const float x = glm::radians(degreeX);
	const float y = glm::radians(degreeY);
	const float z = glm::radians(degreeZ);

	rx.value[0][0] = 1.0f; rx.value[0][1] = 0.0f   ; rx.value[0][2] = 0.0f    ; rx.value[0][3] = 0.0f;
	rx.value[1][0] = 0.0f; rx.value[1][1] = cosf(x); rx.value[1][2] = -sinf(x); rx.value[1][3] = 0.0f;
	rx.value[2][0] = 0.0f; rx.value[2][1] = sinf(x); rx.value[2][2] =  cosf(x); rx.value[2][3] = 0.0f;
	rx.value[3][0] = 0.0f; rx.value[3][1] = 0.0f   ; rx.value[3][2] = 0.0f    ; rx.value[3][3] = 1.0f;

	ry.value[0][0] = cosf(y); ry.value[0][1] = 0.0f; ry.value[0][2] = -sinf(y); ry.value[0][3] = 0.0f;
	ry.value[1][0] = 0.0f   ; ry.value[1][1] = 1.0f; ry.value[1][2] = 0.0f    ; ry.value[1][3] = 0.0f;
	ry.value[2][0] = sinf(y); ry.value[2][1] = 0.0f; ry.value[2][2] =  cosf(y); ry.value[2][3] = 0.0f;
	ry.value[3][0] = 0.0f   ; ry.value[3][1] = 0.0f; ry.value[3][2] = 0.0f    ; ry.value[3][3] = 1.0f;

	rz.value[0][0] = cosf(z); rz.value[0][1] = -sinf(z); rz.value[0][2] = 0.0f; rz.value[0][3] = 0.0f;
	rz.value[1][0] = sinf(z); rz.value[1][1] =  cosf(z); rz.value[1][2] = 0.0f; rz.value[1][3] = 0.0f;
	rz.value[2][0] = 0.0f   ; rz.value[2][1] = 0.0f    ; rz.value[2][2] = 1.0f; rz.value[2][3] = 0.0f;
	rz.value[3][0] = 0.0f   ; rz.value[3][1] = 0.0f    ; rz.value[3][2] = 0.0f; rz.value[3][3] = 1.0f;

	*this = rz * ry * rx;
}

void Matrix4::setRotateTransform(const glm::quat& quaternion) {
	float yy2 = 2.0f * quaternion.y * quaternion.y;
	float xy2 = 2.0f * quaternion.x * quaternion.y;
	float xz2 = 2.0f * quaternion.x * quaternion.z;
	float yz2 = 2.0f * quaternion.y * quaternion.z;
	float zz2 = 2.0f * quaternion.z * quaternion.z;
	float wz2 = 2.0f * quaternion.w * quaternion.z;
	float wy2 = 2.0f * quaternion.w * quaternion.y;
	float wx2 = 2.0f * quaternion.w * quaternion.x;
	float xx2 = 2.0f * quaternion.x * quaternion.x;

	this->value[0][0] = - yy2 - zz2 + 1.0f;
	this->value[0][1] =   xy2 + wz2;
	this->value[0][2] =   xz2 - wy2;
	this->value[0][3] =   0;
	this->value[1][0] =   xy2 - wz2;
	this->value[1][1] = - xx2 - zz2 + 1.0f;
	this->value[1][2] =   yz2 + wx2;
	this->value[1][3] =   0;
	this->value[2][0] =   xz2 + wy2;
	this->value[2][1] =   yz2 - wx2;
	this->value[2][2] = - xx2 - yy2 + 1.0f;
	this->value[2][3] =   0.0f;
	this->value[3][0] =   this->value[3][1] = this->value[3][2] = 0;
	this->value[3][3] =   1.0f;
}


/*------------------------------------------------------------------------------
< set translation transform >
------------------------------------------------------------------------------*/
void Matrix4::setTranslationTransform(float x, float y, float z) {
	this->value[0][0] = 1.0f; this->value[0][1] = 0.0f; this->value[0][2] = 0.0f; this->value[0][3] = x;
	this->value[1][0] = 0.0f; this->value[1][1] = 1.0f; this->value[1][2] = 0.0f; this->value[1][3] = y;
	this->value[2][0] = 0.0f; this->value[2][1] = 0.0f; this->value[2][2] = 1.0f; this->value[2][3] = z;
	this->value[3][0] = 0.0f; this->value[3][1] = 0.0f; this->value[3][2] = 0.0f; this->value[3][3] = 1.0f;
}


/*------------------------------------------------------------------------------
< get transpose >
------------------------------------------------------------------------------*/
Matrix4 Matrix4::getTranspose() const {
	Matrix4 transpose;

	for (unsigned int i = 0 ; i < 4 ; i++) {
		for (unsigned int j = 0 ; j < 4 ; j++) {
			transpose.value[i][j] = this->value[j][i];
		}
	}

	return transpose;
}


/*------------------------------------------------------------------------------
< get glm::mat4 >
------------------------------------------------------------------------------*/
glm::mat4 Matrix4::getGLM() const {
	glm::mat4 result = {
		this->value[0][0], this->value[1][0], this->value[2][0], this->value[3][0],
		this->value[0][1], this->value[1][1], this->value[2][1], this->value[3][1],
		this->value[0][2], this->value[1][2], this->value[2][2], this->value[3][2],
		this->value[0][3], this->value[1][3], this->value[2][3], this->value[3][3]
	};
	return result;
}


/*------------------------------------------------------------------------------
< operator* >
------------------------------------------------------------------------------*/
Matrix4 Matrix4::operator*(const Matrix4& right) const {
	Matrix4 result;

	for (unsigned int i = 0 ; i < 4 ; i++) {
		for (unsigned int j = 0 ; j < 4 ; j++) {
			result.value[i][j] =
			this->value[i][0] * right.value[0][j] +
			this->value[i][1] * right.value[1][j] +
			this->value[i][2] * right.value[2][j] +
			this->value[i][3] * right.value[3][j];
		}
	}

	return result;
}


/*------------------------------------------------------------------------------
< identity >
------------------------------------------------------------------------------*/
Matrix4 Matrix4::identity() {
	Matrix4 identity;
	identity.setIdentity();
	return identity;
}
