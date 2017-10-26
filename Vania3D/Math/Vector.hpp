
#ifndef Vector_hpp
#define Vector_hpp

class Vector3 {
public:
	// value
	float x, y, z;

	Vector3();
	~Vector3();
	// constructor from Assimp matrix
	Vector3(const aiVector3D& assimpVector);
};

#endif /* Vector_hpp */
