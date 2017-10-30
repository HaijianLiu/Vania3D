
#ifndef Animation_hpp
#define Animation_hpp

class Animation {
private:
	Node<Keyframe>* keyframeNode;

	void load(const char* path);
	void processNode(Node<Keyframe>* keyframeNode, const aiNode* ainode, const aiScene* aiscene);

	void processPose(std::vector<Matrix4>& pose, Node<Keyframe>* keyframeNode, const Node<Bone>* node, Matrix4 parentTransformation);

	Vector3 calcInterpolatedScaling(const Keyframe* keyframe);
	Quaternion calcInterpolatedRotation(const Keyframe* keyframe);
	Vector3 calcInterpolatedPosition(const Keyframe* keyframe);
	unsigned int findScaling(const Keyframe* keyframe);
	unsigned int findRotation(const Keyframe* keyframe);
	unsigned int findPosition(const Keyframe* keyframe);

public:
	// animation
	float duration;
	float ticksPerSecond;
	// playback
	float lastStartTimeInSeconds = 0;
	float animationTimeInTicks = 0;
	unsigned int currentPositionIndex = 0;
	unsigned int currentScalingIndex = 0;
	unsigned int currentRotationIndex = 0;
	// blend
	float blendTimeInSeconds = 1.0;

	Animation(const char* path);
	~Animation();

	void updatePose(std::vector<Matrix4>& pose, const Node<Bone>* rootNode, float timeInSeconds);
};

#endif /* Animation_hpp */
