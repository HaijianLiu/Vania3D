
#ifndef Model_hpp
#define Model_hpp

class Model {
private:
	std::vector<Mesh*> meshes;

	unsigned int numBones = 0;
	std::vector<Bone*> bones;
	std::map<std::string, unsigned int> boneMapping; // maps a bone name to its index

	// mRootNode transformation
	Matrix4 globalInverseTransform;

	Node* rootNode;

	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void load(const char* path);
	void processNode(aiNode* ainode, Node* node, const aiScene* aiscene);
	Mesh* processMesh(aiMesh* aimesh, const aiScene* aiscene);

public:
	Model(const char* path);
	~Model();

	// draws the model, and thus all its meshes
	void draw();



	void ReadNodeHeirarchy(float AnimationTime, const Node* node, const Matrix4& ParentTransform);



	std::vector<Matrix4> Transforms;


	const aiScene* m_pScene;

	void BoneTransform(float TimeInSeconds, std::vector<Matrix4>& Transforms);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

};

#endif /* Model_hpp */
