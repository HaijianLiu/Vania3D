
#ifndef Bone_hpp
#define Bone_hpp

class Bone {
public:
	unsigned int index;
	Matrix4 offset;

	Bone();
	~Bone ();
	// constructor from values
	Bone(unsigned int index, Matrix4 offset);
};

#endif /* Bone_hpp */
