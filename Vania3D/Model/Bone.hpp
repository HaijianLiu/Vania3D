
#ifndef Bone_hpp
#define Bone_hpp

class Bone {
public:
	Matrix4 offset;
	Matrix4 transformation;

	Bone();
	~Bone ();
};

#endif /* Bone_hpp */
