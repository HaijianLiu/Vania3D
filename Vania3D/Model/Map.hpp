
#ifndef Map_hpp
#define Map_hpp

class Map {
private:
	Node<glm::mat4>* modelNode;
    std::vector<Material*> materialMapping;

	// loads a model with supported assimp extensions from file and stores the resulting data
	void load(const char* path);
	void processNode(aiNode* ainode, Node<glm::mat4>* node, const aiScene* aiscene);
	void processNode(Node<glm::mat4>* node, glm::mat4 parentTransformation, Game* game, Scene* scene);

public:
	Map(const char* path);
	~Map();

	void createGameObjects(Scene* scene);
};

#endif /* Map_hpp */
