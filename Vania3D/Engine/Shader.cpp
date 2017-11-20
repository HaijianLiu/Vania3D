

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
unsigned int Shader::loadShader(std::string vertexPath, std::string fragmentPath) {

	// create the shaders
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// check error
	checkProgram(vertexShaderID);
	checkProgram(fragmentShaderID);

	// complie
	vertexShaderID = complieShader(vertexShaderID, vertexPath);
	fragmentShaderID = complieShader(fragmentShaderID, fragmentPath);

	// link the program
	unsigned int programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// check error
	checkProgram(programID);

	// detach shader
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	// delete shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}


unsigned int Shader::loadShader(std::string vertexPath, std::string fragmentPath, std::string functionPath) {
	// create the shaders
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// check error
	checkProgram(vertexShaderID);
	checkProgram(fragmentShaderID);

	// complie
	vertexShaderID = complieShader(vertexShaderID, vertexPath);
	fragmentShaderID = complieShader(fragmentShaderID, fragmentPath, functionPath);

	// link the program
	unsigned int programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// check error
	checkProgram(programID);

	// detach shader
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	// delete shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}


/*------------------------------------------------------------------------------
< load uniform location >
------------------------------------------------------------------------------*/
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


/*------------------------------------------------------------------------------
< complie shader >
------------------------------------------------------------------------------*/
unsigned int Shader::complieShader(unsigned int shaderID, std::string path) {
	// read the shader code from the file
	std::string shaderCode = readCode(path);

	// compile code
	const char* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID,1, &sourcePointer, NULL);
	glCompileShader(shaderID);

	return shaderID;
}

unsigned int Shader::complieShader(unsigned int shaderID, std::string path, std::string function) {
	// read the shader code from the file
	std::string shaderCode = readCode(path);
	std::string functionCode = readCode(function);
	shaderCode = shaderCode + functionCode;

	// compile code
	const char* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID,1, &sourcePointer, NULL);
	glCompileShader(shaderID);

	return shaderID;
}


/*------------------------------------------------------------------------------
< check program >
------------------------------------------------------------------------------*/
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


/*------------------------------------------------------------------------------
< read code >
------------------------------------------------------------------------------*/
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
