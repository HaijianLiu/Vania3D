
#ifndef Resources_hpp
#define Resources_hpp

class Resources {
private:
	// Resource storage
	std::unordered_map<const char*, Shader*> shaders;
	std::unordered_map<const char*, Texture*> textures;
	std::unordered_map<const char*, Model*> models;
	std::unordered_map<const char*, LightProbe*> lightProbes;
	std::unordered_map<const char*, Material*> materials;

	Resources();

public:
	static Resources* getInstance();
	~Resources();

	void start();

	Quad* quad;
	Skybox* skybox;

	// shader
	void loadShader(const char* name, const char* vertexPath, const char* fragmentPath);
	Shader* getShader(const char* name);
	// texture
	void loadTexture(const char* name, const char* path);
	Texture* getTexture(const char* name);
	// model
	void loadModel(const char* name, const char* path);
	Model* getModel(const char* name);
	// light probe
	void loadLightProbe(const char* name, const char* path);
	LightProbe* getLightProbe(const char* name);
	// material
	void createMaterial(const char* name, Shader* shader);
	Material* getMaterial(const char* name);
};

#endif /* Resources_hpp */
