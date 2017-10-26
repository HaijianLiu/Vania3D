
#ifndef Vector_hpp
#define Vector_hpp

class Vector3 {
public:
	// value
	float x, y, z;

	Vector3();
	~Vector3();
	// constructor from values
	Vector3(float x, float y, float z);
	// constructor from Assimp matrix
	Vector3(const aiVector3D& assimpVector);

	// operators
	Vector3 operator+(const Vector3& v) const;
	Vector3 operator-(const Vector3& v) const;
	// Vector3 operator*(const float& f) const;
};

Vector3 operator*(const float& f, const Vector3& v);

#endif /* Vector_hpp */
