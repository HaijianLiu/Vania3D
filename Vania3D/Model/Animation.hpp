
#ifndef Animation_hpp
#define Animation_hpp

class Animation {
private:
	// animation info
	float duration;
	float ticksPerSecond;
	// playback
	float lastStartTimeInSeconds = 0;
	float animationTimeInTicks = 0;
	// blend
	float blendTimeInSeconds = 1.0;
	float blendFactor = 1.0;
	// keyframe node tree
	Node<Keyframe>* keyframeNode;

	// load
	void load(const char* path);
	void processNode(Node<Keyframe>* keyframeNode, const aiNode* ainode, const aiScene* aiscene);
	// update pose
	void processPose(std::vector<Matrix4>& pose, Node<Keyframe>* keyframeNode, const Node<Bone>* boneNode, Matrix4 parentTransformation);
	// animation
	void findScaling(Keyframe* keyframe);
	void findRotation(Keyframe* keyframe);
	void findPosition(Keyframe* keyframe);
	glm::vec3 calcInterpolatedScaling(Keyframe* keyframe);
	Quaternion calcInterpolatedRotation(Keyframe* keyframe);
	glm::vec3 calcInterpolatedPosition(Keyframe* keyframe);

public:
	Animation(const char* path);
	~Animation();
	// update model pose data according to time in seconds
	void updatePose(std::vector<Matrix4>& pose, const Node<Bone>* rootNode, float timeInSeconds);
	// restart animation from beginning
	void reset(float timeInSeconds);
};

#endif /* Animation_hpp */
