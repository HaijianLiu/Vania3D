
#ifndef Model_hpp
#define Model_hpp

enum MESH_ATTRIBUTE_TYPE {
	MESH_ATTRIBUTE_DEFAULT, MESH_ATTRIBUTE_INSTANCE, MESH_ATTRIBUTE_INSTANCE_ANIMATION, MESH_ATTRIBUTE_BONE
};

class Model {
	friend class FrustumCulling;
	friend class MaterialLayer;
	friend class MeshRenderer;

private:
	Node<Bone>* boneNode;
	std::vector<Mesh*> meshes;
	std::vector<Animation*> animations;
	std::vector<glm::mat4> pose;
	unsigned int attributeType;
	unsigned int currentAnimation = 0;

	// loads a model with supported assimp extensions from file and stores the resulting data
	void load(const char* path);
	void processNode(aiNode* ainode, Node<Bone>* node, const aiScene* aiscene);
	void createMesh(aiMesh* aimesh, const aiScene* aiscene);

public:
	std::vector<std::string> materialNames;
	bool distanceCulling = false;

	
	Model();
	Model(unsigned int attributeType, const char* path);
	~Model();

	void addAnimation(Animation* animation);
	void updatePose(unsigned int animationIndex, float timeInSeconds);

	void draw();
	void drawMesh(unsigned int meshIndex);
	void drawBounding();
	void setPoseUniform(Shader* shader);

	static unsigned int createBox(glm::vec3 boundingMax, glm::vec3 boundingMin);
};

#endif /* Model_hpp */
