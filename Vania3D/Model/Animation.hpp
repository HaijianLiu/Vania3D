
#ifndef Animation_hpp
#define Animation_hpp

class Animation {
private:
	Node<Keyframe>* keyframeNode;

	void load(const char* path);
	void processNode(Node<Keyframe>* keyframeNode, const aiNode* ainode, const aiScene* aiscene);

	void processPose(std::vector<Matrix4>& pose, Node<Keyframe>* keyframeNode, const Node<Bone>* node, Matrix4 parentTransformation, float animationTimeInTicks);
	void calcInterpolatedScaling(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	void calcInterpolatedRotation(Quaternion& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	void calcInterpolatedPosition(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	unsigned int findScaling(float animationTimeInTicks, const Keyframe* keyframe) const;
	unsigned int findRotation(float animationTimeInTicks, const Keyframe* keyframe) const;
	unsigned int findPosition(float animationTimeInTicks, const Keyframe* keyframe) const;

public:
	float duration;
	float ticksPerSecond;
	float lastStartTimeInSeconds = 0;
	float blendTimeInSeconds = 1.0;

	Animation(const char* path);
	~Animation();

	void updatePose(std::vector<Matrix4>& pose, const Node<Bone>* rootNode, float timeInSeconds);
};

#endif /* Animation_hpp */
