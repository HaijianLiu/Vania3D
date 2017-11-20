

#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Shader::Shader(std::string vertexPath, std::string fragmentPath) {
	this->programID = loadShader(vertexPath, fragmentPath);
	this->loadUniformLocation(vertexPath.c_str());
	this->loadUniformLocation(fragmentPath.c_str());
}

Shader::Shader(std::string vertexPath, std::string fragmentPath, std::string functionPath) {
	this->programID = loadShader(vertexPath, fragmentPath, functionPath);
	this->loadUniformLocation(vertexPath.c_str());
	this->loadUniformLocation(fragmentPath.c_str());
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Shader::~Shader() {
	glDeleteProgram(this->programID);
}


/*------------------------------------------------------------------------------
< Set Functions >
------------------------------------------------------------------------------*/
// activate the shader
void Shader::use() {
	glUseProgram(this->programID);
}

void Shader::getUniformLocation(const char* name) {
	int uniformLocation = glGetUniformLocation(this->programID, name);
	std::string nameString = std::string(name);
	this->uniformLocations.insert(std::make_pair(nameString, uniformLocation));
//	this->uniformLocations.insert(std::make_pair("1", 2));
//	this->uniformLocations.insert(std::make_pair("2", 2));
//	this->uniformLocations.insert(std::make_pair("3", 2));
}

// utility uniform functions
void Shader::setBool(const char* name, bool value) {
	if (this->uniformLocations.find(name) != this->uniformLocations.end())
		glUniform1i(this->uniformLocations[name], (int)value );
	else
		std::cout << name << std::endl;
}
void Shader::setInt(const char* name, int value) {
	if (this->uniformLocations.find(name) != this->uniformLocations.end())
		glUniform1i(this->uniformLocations[name], value);
	else
		std::cout << name << std::endl;
}
void Shader::setFloat(const char* name, float value) {
	if (this->uniformLocations.find(name) != this->uniformLocations.end())
		glUniform1f(this->uniformLocations[name], value);
	else
		std::cout << name << std::endl;
}
void Shader::setVec2(const char* name, glm::vec2 value) {
	if (this->uniformLocations.find(name) != this->uniformLocations.end())
		glUniform2fv(this->uniformLocations[name], 1, &value[0]);
	else
		std::cout << name << std::endl;
}
void Shader::setVec3(const char* name, glm::vec3 value) {
	if (this->uniformLocations.find(name) != this->uniformLocations.end())
		glUniform3fv(this->uniformLocations[name], 1, &value[0]);
	else
		std::cout << name << std::endl;
}
void Shader::setVec4(const char* name, glm::vec4 value) {
	if (this->uniformLocations.find(name) != this->uniformLocations.end())
		glUniform4fv(this->uniformLocations[name], 1, &value[0]);
	else
		std::cout << name << std::endl;
}
void Shader::setMat2(const char* name, glm::mat2 mat) {
	if (this->uniformLocations.find(name) != this->uniformLocations.end())
		glUniformMatrix2fv(this->uniformLocations[name], 1, GL_FALSE, &mat[0][0]);
	else
		std::cout << name << std::endl;
}
void Shader::setMat3(const char* name, glm::mat3 mat) {
	if (this->uniformLocations.find(name) != this->uniformLocations.end())
		glUniformMatrix3fv(this->uniformLocations[name], 1, GL_FALSE, &mat[0][0]);
	else
		std::cout << name << std::endl;
}
void Shader::setMat4(const char* name, glm::mat4 mat) {
	if (this->uniformLocations.find(name) != this->uniformLocations.end())
		glUniformMatrix4fv(this->uniformLocations[name], 1, GL_FALSE, &mat[0][0]);
	else
		std::cout << name << std::endl;
}


/*------------------------------------------------------------------------------
< loadShader >
------------------------------------------------------------------------------*/
// load Shader
unsigned int Shader::loadShader(std::string vertexPath, std::string fragmentPath) {

	// Create the shaders
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
	// complie
	vertexShaderID = complieShader(vertexShaderID, vertexPath);
	fragmentShaderID = complieShader(fragmentShaderID, fragmentPath);
	
	// Link the program
	unsigned int programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	
	// check error
	checkProgram(programID);

	// Detach shader
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	// Delete shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}


unsigned int Shader::loadShader(std::string vertexPath, std::string fragmentPath, std::string functionPath) {

	// Create the shaders
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertexPath, std::ios::in);
	if (vertexShaderStream.is_open()) {
		std::string line = "";
		while (getline(vertexShaderStream, line)) {
			vertexShaderCode += "\n" + line;
		}
		vertexShaderStream.close();
	} else {
		printf("[Shader] impossible to open: <%s>\n", vertexPath.c_str());
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::ifstream functionShaderStream(functionPath, std::ios::in);
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragmentPath, std::ios::in);
	if (fragmentShaderStream.is_open()) {
		std::string line = "";
		while (getline(fragmentShaderStream,line)) {
			fragmentShaderCode += "\n" + line;
		}
		fragmentShaderStream.close();
	} else {
		printf("[Shader] impossible to open: <%s>\n",fragmentPath.c_str());
		return 0;
	}
	if (functionShaderStream.is_open()) {
		std::string line = "";
		while (getline(functionShaderStream, line)) {
			fragmentShaderCode += "\n" + line;
		}
		functionShaderStream.close();
	} else {
		printf("[Shader] impossible to open: <%s>\n", functionPath.c_str());
		return 0;
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	const char* vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID,1,&vertexSourcePointer,NULL);
	glCompileShader(vertexShaderID);
	// Check Vertex Shader
	glGetShaderiv(vertexShaderID,GL_COMPILE_STATUS,&result);
	glGetShaderiv(vertexShaderID,GL_INFO_LOG_LENGTH,&infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(vertexShaderID,infoLogLength,NULL,&vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	const char* fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID,1,&fragmentSourcePointer,NULL);
	glCompileShader(fragmentShaderID);
	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID,GL_COMPILE_STATUS,&result);
	glGetShaderiv(fragmentShaderID,GL_INFO_LOG_LENGTH,&infoLogLength);
	if (infoLogLength > 0){
		std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(fragmentShaderID,infoLogLength,NULL,&fragmentShaderErrorMessage[0]);
		printf("%s\n",&fragmentShaderErrorMessage[0]);
	}

	// Link the program
	unsigned int programID = glCreateProgram();
	glAttachShader(programID,vertexShaderID);
	glAttachShader(programID,fragmentShaderID);
	glLinkProgram(programID);
	// Check the program
	glGetProgramiv(programID,GL_LINK_STATUS,&result);
	glGetProgramiv(programID,GL_INFO_LOG_LENGTH,&infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID,infoLogLength,NULL,&programErrorMessage[0]);
		printf("%s\n",&programErrorMessage[0]);
	}

	// Detach shader
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	// Delete shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

void Shader::loadUniformLocation(const char* path) {
	FILE* file = fopen(path, "rb");
	char lineHeader[128], restOfLine[1024];

	while (true) {
		// read the first word of the line
		if (fscanf(file, "%s", lineHeader) == EOF) break;

		// parse lineHeader
		if (strcmp(lineHeader, "uniform") == 0) {
			fscanf(file, "%s", lineHeader); // skip type
			fscanf(file, "%s", lineHeader);
			std::string uniformName = std::string(lineHeader);

			// if uniform is array
			if (uniformName.rfind("[") < uniformName.length()) {
				std::string sizeString = uniformName.substr(uniformName.find("[") + 1, uniformName.find("]") - uniformName.find("[") - 1);
				uniformName = uniformName.substr(0, uniformName.rfind("["));
				unsigned int size = std::stoi(sizeString);

				for (unsigned int i = 0; i < size; i++) {
					this->getUniformLocation((uniformName + "[" + std::to_string(i) + "]").c_str());
				}
			}
			else {
				uniformName = uniformName.substr(0, uniformName.rfind(";"));
				this->getUniformLocation(uniformName.c_str());
			}
		}
		else {
			fgets(restOfLine, 1024, file); // eat up the rest of the line
		}
	}

	fclose(file);
}

unsigned int Shader::complieShader(unsigned int shaderID, std::string path) {
	// read the shader code from the file
	std::string shaderCode;
	std::ifstream shaderStream(path, std::ios::in);
	if (shaderStream.is_open()) {
		std::string line = "";
		while (getline(shaderStream, line)) {
			shaderCode += "\n" + line;
		}
		shaderStream.close();
	} else {
		printf("[Shader] impossible to open: <%s>\n", path.c_str());
		return shaderID;
	}
	
	checkProgram(shaderID);
	
	// compile shader
	const char* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID,1, &sourcePointer, NULL);
	glCompileShader(shaderID);
	
	return shaderID;
}

void Shader::checkProgram(unsigned int shaderID) {
	GLint result = GL_FALSE;
	int infoLogLength;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> shaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
		printf("%s\n", &shaderErrorMessage[0]);
	}
}

std::string Shader::readCode(std::string path) {
	// read code from file
	std::string code;
	std::ifstream stream(path, std::ios::in);
	if (stream.is_open()) {
		std::string line = "";
		while (getline(stream, line)) {
			code += "\n" + line;
		}
		stream.close();
	} else {
		printf("[Shader] impossible to open: <%s>\n", path.c_str());
		return NULL;
	}
	
	return code;
}
