
#ifndef Model_hpp
#define Model_hpp

class Model {
private:
	Node<Matrix4>* rootNode;
	std::vector<Mesh*> meshes;
	std::vector<Animation*> animations;

	std::vector<Bone*> bones;
	std::map<std::string, unsigned int> boneMapping; // maps a bone name to its index


	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void load(const char* path);
	void processNode(aiNode* ainode, Node<Matrix4>* node, const aiScene* aiscene);
	Mesh* processMesh(aiMesh* aimesh, const aiScene* aiscene);
	void processAnimation(const aiScene* aiscene);
	void processPose(const Animation* animation, float animationTimeInTicks, const Node<Matrix4>* node, const Matrix4& parentTransformation);

public:
	std::vector<Matrix4> pose;

	Model(const char* path);
	~Model();

	void updatePose(unsigned int animationIndex, float time); // time in seconds
	const Keyframe* FindNodeAnim(const Animation* animation, const std::string nodeName);

	// draws the model, and thus all its meshes
	void draw();



	// const aiScene* m_pScene;

	// void BoneTransform(float TimeInSeconds, std::vector<Matrix4>& Transforms);
	void CalcInterpolatedScaling(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe);
	void CalcInterpolatedRotation(Quaternion& Out, float animationTimeInTicks, const Keyframe* keyframe);
	void CalcInterpolatedPosition(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe);
	uint FindScaling(float animationTimeInTicks, const Keyframe* keyframe);
	uint FindRotation(float animationTimeInTicks, const Keyframe* keyframe);
	uint FindPosition(float animationTimeInTicks, const Keyframe* keyframe);

};

#endif /* Model_hpp */
