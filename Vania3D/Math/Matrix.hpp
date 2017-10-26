
#ifndef Matrix_hpp
#define Matrix_hpp

class Matrix4 {
public:
	// value
	float value[4][4];

	// constructor
	Matrix4();
	Matrix4(float a00, float a01, float a02, float a03,
	        float a10, float a11, float a12, float a13,
	        float a20, float a21, float a22, float a23,
	        float a30, float a31, float a32, float a33);
	~Matrix4();
	// constructor from Assimp matrix
	Matrix4(const aiMatrix4x4& assimpMatrix);
	Matrix4(const aiMatrix3x3& assimpMatrix);

	// set functions
	void setZero();
	void setIdentity();
	void setScaleTransform(float scaleX, float scaleY, float scaleZ);
	void setRotateTransform(float degreeX, float degreeY, float degreeZ);
	void setRotateTransform(const Quaternion& quaternion);
	void setTranslationTransform(float x, float y, float z);

	// get functions
	Matrix4 getTranspose() const;
	glm::mat4 getGLM() const;

	// operators
	Matrix4 operator*(const Matrix4& right) const;
};

#endif /* Matrix_hpp */
