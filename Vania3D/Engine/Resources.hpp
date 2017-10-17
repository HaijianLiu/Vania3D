
#ifndef Resources_hpp
#define Resources_hpp

class Resources {
private:
	// Resource storage
	std::map<const char*, Shader*> shaders;
	std::map<const char*, Texture*> textures;
	// std::map<const char*, Audio*> audios;
	std::map<const char*, Model*> models;

public:
	Resources();
	~Resources();

	void start();

	// Loads (and generates) a shader program from file loading vertex, fragment
	void loadShader(const char* name, const char* vertexPath, const char* fragmentPath);
	Shader* getShader(const char* name);
	// Loads (and generates) a texture from file
	void loadTexture(const char* name, const char* path);
	Texture* getTexture(const char* name);
	// Loads (and generates) a model from file
	void loadModel(const char* name, const char* path);
	Model* getModel(const char* name);
	// Create a Audio with path
	// void loadAudio(const char* name, const char* path);
	// Audio* getAudio(const char* name);
};

#endif /* Resources_hpp */
