
#ifndef Quaternion_hpp
#define Quaternion_hpp

class Quaternion {
public:
	// value
	float x, y, z, w;

	// constructor
	Quaternion();
	~Quaternion ();
	// constructor from values
	Quaternion(float x, float y, float z, float w);
	// constructor from Assimp Quaternion
	Quaternion(const aiQuaternion& assimpQuaternion);

	// functions
	void normalize();
	glm::vec3 degrees();

	// get functions
	aiQuaternion getAissmp() const;

	// operators
	Quaternion operator*(const Quaternion& right) const;
	Quaternion operator*(const glm::vec3& vector) const;
};

#endif /* Quaternion_hpp */
