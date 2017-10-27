
#ifndef Model_hpp
#define Model_hpp

class Model {
private:
	Node<Matrix4>* rootNode;
	std::vector<Mesh*> meshes;
	std::vector<Animation*> animations;

	std::unordered_map<std::string, Bone> bones;


	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void load(const char* path);
	void processNode(aiNode* ainode, Node<Matrix4>* node, const aiScene* aiscene);
	void processAnimation(const aiScene* aiscene);
	Mesh* createMesh(aiMesh* aimesh, const aiScene* aiscene);

public:
	std::vector<Matrix4> pose;

	Model(const char* path);
	~Model();

	void updatePose(unsigned int animationIndex, float time); // time in seconds

	// draws the model, and thus all its meshes
	void draw();
};

#endif /* Model_hpp */
