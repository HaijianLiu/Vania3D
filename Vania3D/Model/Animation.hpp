
#ifndef Animation_hpp
#define Animation_hpp

class Animation {
private:
	Node<Keyframe>* keyframeNode;

	void load(const char* path);
	void processNode(Node<Keyframe>* keyframeNode, const aiNode* ainode, const aiScene* aiscene);

	void processPose(std::vector<Matrix4>& pose, Node<Keyframe>* keyframeNode, const Node<Bone>* node, Matrix4 parentTransformation);

	Vector3 calcInterpolatedScaling(Keyframe* keyframe);
	Quaternion calcInterpolatedRotation(Keyframe* keyframe);
	Vector3 calcInterpolatedPosition(Keyframe* keyframe);
	void findScaling(Keyframe* keyframe);
	void findRotation(Keyframe* keyframe);
	void findPosition(Keyframe* keyframe);

public:
	// animation
	float duration;
	float ticksPerSecond;
	// playback
	float lastStartTimeInSeconds = 0;
	float animationTimeInTicks = 0;
	// blend
	float blendTimeInSeconds = 1.0;

	Animation(const char* path);
	~Animation();

	void updatePose(std::vector<Matrix4>& pose, const Node<Bone>* rootNode, float timeInSeconds);
};

#endif /* Animation_hpp */
