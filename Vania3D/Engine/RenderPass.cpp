
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
RenderPass::RenderPass() {
	// setup plane vao
	unsigned int vbo;
	float vertices[] = {
		// vertex             // texCoord
		-1.0f,  1.0f, 0.0f,   0.0f, 1.0f, // left top
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // left bottom
		 1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // right top
		 1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // right bottom
	};
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}


/*------------------------------------------------------------------------------
< Desstructor >
------------------------------------------------------------------------------*/
RenderPass::~RenderPass() {

}


/*------------------------------------------------------------------------------
< init >
------------------------------------------------------------------------------*/
void RenderPass::init(Shader* shader, unsigned int number) {

	this->shader = shader;

	this->frameBuffer = createFrameBuffer(number);

	// Set Shader
	this->shader->use();
	std::string passes = UNIFORM_TEX_PASSES;
	for (unsigned i = 0; i < number; i++) {
		this->shader->setInt((passes + "[" + std::to_string(i) + "]").c_str(), i);
	}
	this->shader->setInt(UNIFORM_TEX_SHADOW, 13);
}


/*------------------------------------------------------------------------------
< render >
------------------------------------------------------------------------------*/
void RenderPass::render(RenderLayer* renderLayer, std::vector<GameObject*>* pointLights, GameObject* camera) {
	Game* game = Game::getInstance();

	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw model
	renderLayer->render(camera);
	// reset framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// bind shader
	this->shader->use();

	// camera
	this->shader->setVec3(UNIFORM_VEC3_CAMERA_POSITION, camera->getComponent<Transform>()->position);
	// lights
	std::string lightPositions = UNIFORM_VEC3_LIGHT_POSITION;
	std::string lightColors = UNIFORM_VEC3_LIGHT_COLOR;
	for (unsigned int i = 0; i < pointLights->size(); i++) {
		this->shader->setVec3((lightPositions + "[" + std::to_string(i) + "]").c_str(), pointLights->at(i)->getComponent<Transform>()->position);
		this->shader->setVec3((lightColors + "[" + std::to_string(i) + "]").c_str(), pointLights->at(i)->getComponent<PointLight>()->color);
	}
	// shadows
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, game->shadowMapping->depthMap);
	this->shader->setMat4(UNIFORM_MATRIX_LIGHTSPACE, game->shadowMapping->lightSpace);

	// render
	for (unsigned int i = 0; i < this->frameBuffer.passes.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->frameBuffer.passes[i]);
	}

	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


void RenderPass::renderBounding(std::vector<MeshRenderer*>* renderQueue, GameObject* camera) {
	Game* game = Game::getInstance();

	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// render bounding box in line mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Shader* shader = game->resources->getShader("simple");
	shader->use();
	camera->getComponent<Camera>()->setUniforms(shader);
	for (unsigned int i = 0; i < renderQueue->size(); i++) {
		Transform* transform = renderQueue->at(i)->gameObject->getComponent<Transform>();
		transform->setUniform(shader);
		renderQueue->at(i)->model->drawBounding();
	}

	// reset framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// bind shader
	game->resources->getShader("renderpass_color_1_passes")->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->frameBuffer.passes[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// glBindVertexArray(0);
}




/*------------------------------------------------------------------------------
< set active light probe >
------------------------------------------------------------------------------*/
void RenderPass::setActiveLightProbe(LightProbe* lightProbe) {
	this->shader->use();
	// ibl
	this->shader->setInt(UNIFORM_TEX_IRRADIANCE, 10);
	this->shader->setInt(UNIFORM_TEX_PREFILTER, 11);
	this->shader->setInt(UNIFORM_TEX_BRDFLUT, 12);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lightProbe->irradiance);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lightProbe->prefilter);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, lightProbe->brdf);
}


FrameBuffer RenderPass::createFrameBuffer(unsigned int number) {
	Game* game = Game::getInstance();
	FrameBuffer frameBuffer;

	// configure (floating point) framebuffers
	glGenFramebuffers(1, &frameBuffer.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.fbo);

	// create 2 floating point color buffers
	unsigned int colorBuffers[number];
	glGenTextures(number, colorBuffers);
	for (unsigned int i = 0; i < number; i++) {
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, game->window->screenWidth, game->window->screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
		frameBuffer.passes.push_back(colorBuffers[i]);
	}

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	unsigned int attachments[number];
	for (unsigned int i = 0; i < number; i++) {
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(number, attachments);

	// create and attach depth buffer (renderbuffer)
	unsigned int depthRBO;
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, game->window->screenWidth, game->window->screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER,0);

	return frameBuffer;
}
