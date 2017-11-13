
#ifndef Map_hpp
#define Map_hpp

struct ModelProperties {
	glm::mat4 nodeTransformation;
	std::vector<unsigned int> materialIndices;
};

class Map {
private:
	Node<ModelProperties>* modelNode;
	std::vector<Material*> materialMapping;

	// loads a model with supported assimp extensions from file and stores the resulting data
	void load(const char* path);
	void processNode(aiNode* ainode, Node<ModelProperties>* node, const aiScene* aiscene);
	void processNode(Node<ModelProperties>* node, glm::mat4 parentTransformation, Game* game, Scene* scene);

public:
	Map(const char* path);
	~Map();

	void createGameObjects(Scene* scene);
};

#endif /* Map_hpp */
