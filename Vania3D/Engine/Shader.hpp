
#ifndef Shader_hpp
#define Shader_hpp

class Shader {
private:
	unsigned int loadShader(std::string vertexPath, std::string fragmentPath);

public:
	unsigned int programID;

	// constructor generates the shader on the fly
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	// activate the shader
	void use();
	// utility uniform functions
	void setBool(const char* name, bool value);
	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void setVec2(const char* name, glm::vec2 value);
	void setVec2(const char* name, float x, float y);
	void setVec3(const char* name, glm::vec3 value);
	void setVec3(const char* name, float x, float y, float z);
	void setVec4(const char* name, glm::vec4 value);
	void setVec4(const char* name, float x, float y, float z, float w);
	void setMat2(const char* name, glm::mat2 mat);
	void setMat3(const char* name, glm::mat3 mat);
	void setMat4(const char* name, glm::mat4 mat);
};

#endif /* Shader_hpp */
