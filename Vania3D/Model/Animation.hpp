
#ifndef Animation_hpp
#define Animation_hpp

class Animation {
private:
	Node<Keyframe*>* keyframeNode;

	void load(const char* path);
	void processNode(Node<Keyframe*>* keyframeNode, const aiNode* ainode, const aiScene* aiscene);

	void processPose(std::vector<Matrix4>& pose, Node<Keyframe*>* keyframeNode, const Node<Matrix4>* node, const std::unordered_map<std::string, Bone>* bones, Matrix4 parentTransformation, float animationTimeInTicks);
	void CalcInterpolatedScaling(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	void CalcInterpolatedRotation(Quaternion& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	void CalcInterpolatedPosition(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	uint FindScaling(float animationTimeInTicks, const Keyframe* keyframe) const;
	uint FindRotation(float animationTimeInTicks, const Keyframe* keyframe) const;
	uint FindPosition(float animationTimeInTicks, const Keyframe* keyframe) const;

public:
	float duration;
	float ticksPerSecond;
	float lastStartTimeInSeconds = 0;

	Animation(const char* path);
	~Animation();

	void updatePose(std::vector<Matrix4>& pose, const Node<Matrix4>* rootNode, const std::unordered_map<std::string, Bone>* bones, float timeInSeconds);
};

#endif /* Animation_hpp */
