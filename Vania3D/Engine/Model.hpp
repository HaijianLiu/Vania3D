
#ifndef Model_hpp
#define Model_hpp

class Model {
private:
	std::vector<Mesh*> meshes;

	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void load(const char* path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

public:
	Model(const char* path);
	~Model();

	// draws the model, and thus all its meshes
	void draw();
};

#endif /* Model_hpp */
