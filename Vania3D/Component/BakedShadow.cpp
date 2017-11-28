
#include "Engine.hpp"

BakedShadow::BakedShadow() {

}

BakedShadow::~BakedShadow() {

}

void BakedShadow::bake(std::vector<MeshRenderer*>* renderQueue) {
	PointLight* pointLight = this->gameObject->getComponent<PointLight>();
	float near = 0.1f;
	float far = pointLight->radius;
	glm::vec3 lightPosition = this->gameObject->transform->position;
	
	glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), 1.0f, near, far);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)));
	
	glViewport(0, 0, this->bakeSize, this->bakeSize);
	glBindFramebuffer(GL_FRAMEBUFFER, this->bakedShadowMap.fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	this->shader->use();
	for (unsigned int i = 0; i < 6; ++i)
		this->shader->setMat4(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);
	this->shader->setFloat("lightRadius", far);
	this->shader->setVec3("lightPosition", lightPosition);
	
	
	for (unsigned int i = 0; i < renderQueue->size(); i++) {
		if (renderQueue->at(i)->gameObject->staticObject) {
			renderQueue->at(i)->gameObject->transform->setUniform(this->shader);
			renderQueue->at(i)->model->draw();
		}
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BakedShadow::start() {
	glGenFramebuffers(1, &this->bakedShadowMap.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->bakedShadowMap.fbo);
	unsigned int shadowCubeMap = RenderPass::createCubeAttachment(GL_DEPTH_COMPONENT, this->bakeSize);
	this->bakedShadowMap.textures.push_back(shadowCubeMap);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->shader = this->game->resources->getShader("BakedShadow");
	if (this->shader == nullptr) this->compileShader();
}

void BakedShadow::compileShader() {
	// create the shaders
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	
	// complie
	std::vector<std::string> vertexPaths, fragmentPaths, geometryPaths;
	vertexPaths.push_back("./Assets/Shaders/Vertex/baked_1_locations_shadowmapping.vs.glsl");
	fragmentPaths.push_back("./Assets/Shaders/Fragment/baked_cubemap_shadowmapping.fs.glsl");
	geometryPaths.push_back("./Assets/Shaders/Geometry/baked_cubemap_shadowmapping.gs.glsl");
	vertexShaderID = Shader::complieShader(vertexShaderID, &vertexPaths);
	fragmentShaderID = Shader::complieShader(fragmentShaderID, &fragmentPaths);
	geometryShaderID = Shader::complieShader(geometryShaderID, &geometryPaths);
	
	Shader::checkProgram(vertexShaderID);
	Shader::checkProgram(fragmentShaderID);
	Shader::checkProgram(geometryShaderID);
	
	// link the program
	unsigned int programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glAttachShader(programID, geometryShaderID);
	glLinkProgram(programID);
	
	// check error
	Shader::checkProgram(programID);
	
	// detach shader
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDetachShader(programID, geometryShaderID);
	// delete shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteShader(geometryShaderID);
	
	
	Shader* shader = new Shader();
	shader->loadUniformLocation(vertexPaths[0].c_str());
	shader->loadUniformLocation(fragmentPaths[0].c_str());
	shader->loadUniformLocation(geometryPaths[0].c_str());
	shader->programID = programID;
	
	this->game->resources->shaders.insert(std::make_pair("BakedShadow", shader));
	this->shader = shader;
}
