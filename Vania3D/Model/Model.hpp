
#ifndef Model_hpp
#define Model_hpp

class Model {
private:
	Node<Bone>* boneNode;
	std::vector<Mesh*> meshes;
	std::vector<Animation*> animations;

	unsigned int currentAnimation = 0;

	// loads a model with supported assimp extensions from file and stores the resulting data
	void load(const char* path);
	void processNode(aiNode* ainode, Node<Bone>* node, const aiScene* aiscene);
	void createMesh(aiMesh* aimesh, const aiScene* aiscene);

public:
	std::vector<Matrix4> pose;

	Model(const char* path);
	~Model();

	void addAnimation(Animation* animation);
	void updatePose(unsigned int animationIndex, float timeInSeconds);

	// draws the model, and thus all its meshes
	void draw();
};

#endif /* Model_hpp */
