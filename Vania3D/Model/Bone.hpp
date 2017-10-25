
#ifndef Bone_hpp
#define Bone_hpp

class Bone {
private:
	glm::mat4 offset;

public:
	Bone();
	~Bone ();

	glm::mat4 aiMatrixToGLMatrix4(aiMatrix4x4 matrix);
};

#endif /* Bone_hpp */
