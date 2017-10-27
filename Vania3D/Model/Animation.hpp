
#ifndef Animation_hpp
#define Animation_hpp

class Animation {
private:
	void CalcInterpolatedScaling(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	void CalcInterpolatedRotation(Quaternion& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	void CalcInterpolatedPosition(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	uint FindScaling(float animationTimeInTicks, const Keyframe* keyframe) const;
	uint FindRotation(float animationTimeInTicks, const Keyframe* keyframe) const;
	uint FindPosition(float animationTimeInTicks, const Keyframe* keyframe) const;

public:
	std::string name;
	float duration = 1.0;
	float ticksPerSecond = 25.0;

	Node<Keyframe*>* keyframeNode;

	std::vector<Keyframe*> keyframes;

	Animation();
	~Animation();

	void processNode(Node<Keyframe*>* keyframeNode, const Node<Matrix4>* node, const aiAnimation* aianimation);
	void copyNodeTree(const Node<Matrix4>* rootNode, const aiAnimation* aianimation);

	void processPose(std::vector<Matrix4>& pose, Node<Keyframe*>* keyframeNode, const Node<Matrix4>* node, const std::unordered_map<std::string, Bone>* bones, Matrix4 parentTransformation, float animationTimeInTicks);
	void updatePose(std::vector<Matrix4>& pose, const Node<Matrix4>* rootNode, const std::unordered_map<std::string, Bone>* bones, float timeInSeconds);
};

#endif /* Animation_hpp */
