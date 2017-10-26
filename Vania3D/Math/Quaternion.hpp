
#ifndef Quaternion_hpp
#define Quaternion_hpp

class Quaternion {
public:
	float x, y, z, w;

	Quaternion(float x, float y, float z, float w);
	~Quaternion ();

	void normalize();
	glm::vec3 degrees();

	Quaternion operator*(const Quaternion& right) const;
	Quaternion operator*(const glm::vec3& vector) const;
};

#endif /* Quaternion_hpp */
