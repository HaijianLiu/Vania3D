
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
	unsigned int currentPositionIndex = 0;
	unsigned int currentRotationIndex = 0;
	unsigned int currentScalingIndex = 0;
	std::vector<VectorKey> positionKeys;
	std::vector<QuaternionKey> rotationKeys;
	std::vector<VectorKey> scalingKeys;

	Keyframe();
	~Keyframe();
};

#endif /* Keyframe_hpp */