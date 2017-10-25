
#ifndef Quaternion_hpp
#define Quaternion_hpp

class Quaternion {
public:
	float x, y, z, w;

	Quaternion(float x, float y, float z, float w);
	~Quaternion ();

	void normalize();
	glm::vec3 degrees();
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);
Quaternion operator*(const Quaternion& q, const glm::vec3& v);

#endif /* Quaternion_hpp */
