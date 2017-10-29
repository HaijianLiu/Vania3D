
#ifndef Model_hpp
#define Model_hpp

class Model {
private:
	Node<Matrix4>* rootNode;
	std::vector<Mesh*> meshes;
	std::unordered_map<std::string, Bone> bones; // to be in boneNode

	// loads a model with supported assimp extensions from file and stores the resulting data
	void load(const char* path);
	void processNode(aiNode* ainode, Node<Matrix4>* node, const aiScene* aiscene);
	void createMesh(aiMesh* aimesh, const aiScene* aiscene);

public:
	std::vector<Matrix4> pose;
	std::vector<Animation*> animations;

	Model(const char* path);
	~Model();

	void addAnimation(std::string name, Animation* animation);
	void updatePose(unsigned int animationIndex, float timeInSeconds);

	// draws the model, and thus all its meshes
	void draw();
};

#endif /* Model_hpp */
