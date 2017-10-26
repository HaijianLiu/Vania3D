
#ifndef Keyframe_hpp
#define Keyframe_hpp

struct VectorKey {
	float time;
	Vector3 value;
};

struct QuaternionKey {
	float time;
	Quaternion value;
};

class Keyframe {
public:
	std::string nodeName;
	std::vector<VectorKey> positionKeys;
	std::vector<QuaternionKey> rotationKeys;
	std::vector<VectorKey> scalingKeys;

	Keyframe();
	~Keyframe();
};

#endif /* Keyframe_hpp */
