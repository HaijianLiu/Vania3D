
#ifndef Shader_hpp
#define Shader_hpp

class Shader {
	friend class RenderPass;

	std::vector<std::string> vertexPaths, fragmentPaths;
	std::unordered_map<std::string, int> uniformLocations;
	
	void loadUniformLocation(const char* path);
	void getUniformLocation(const char* name);
	
	static void checkProgram(unsigned int shaderID);
	static std::string readCode(std::string path);
	static unsigned int compileShader(unsigned int shaderID, std::vector<std::string>* paths);
	
	void updateSystemUniforms(Scene* scene);

public:
	unsigned int programID;

	Shader();
	~Shader();

	// add codes before complie
	void addVertexCode(std::string path);
	void addFragmentCode(std::string path);
	void compile();

	// activate the shader
	void use();

	// utility uniform functions
	void setBool(const char* name, bool value);
	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void setVec2(const char* name, glm::vec2 value);
	void setVec3(const char* name, glm::vec3 value);
	void setVec4(const char* name, glm::vec4 value);
	void setMat2(const char* name, glm::mat2 mat);
	void setMat3(const char* name, glm::mat3 mat);
	void setMat4(const char* name, glm::mat4 mat);
};

#endif /* Shader_hpp */
