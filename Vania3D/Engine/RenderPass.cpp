
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
RenderPass::RenderPass() {

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
	Game* game = Game::getInstance();

	this->vao = game->resources->quad->vao;
	this->deferredPBR = shader;
	this->bufferG = createFrameBuffer(number);

	// Set Shader
	this->deferredPBR->use();
	std::string passes = UNIFORM_TEX_PASSES;
	for (unsigned i = 0; i < number; i++) {
		this->deferredPBR->setInt((passes + "[" + std::to_string(i) + "]").c_str(), i);
	}
	this->deferredPBR->setInt(UNIFORM_TEX_SHADOW, 13);
}


/*------------------------------------------------------------------------------
< render >
------------------------------------------------------------------------------*/
void RenderPass::render(RenderLayer* renderLayer, std::vector<GameObject*>* pointLights, GameObject* camera) {
	Game* game = Game::getInstance();

	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->bufferG.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw model
	renderLayer->render(camera);
	// reset framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// bind shader
	this->deferredPBR->use();

	// camera
	this->deferredPBR->setVec3(UNIFORM_VEC3_CAMERA_POSITION, camera->getComponent<Transform>()->position);
	// lights
	std::string lightPositions = UNIFORM_VEC3_LIGHT_POSITION;
	std::string lightColors = UNIFORM_VEC3_LIGHT_COLOR;
	for (unsigned int i = 0; i < pointLights->size(); i++) {
		this->deferredPBR->setVec3((lightPositions + "[" + std::to_string(i) + "]").c_str(), pointLights->at(i)->getComponent<Transform>()->position);
		this->deferredPBR->setVec3((lightColors + "[" + std::to_string(i) + "]").c_str(), pointLights->at(i)->getComponent<PointLight>()->color);
	}
	// shadows
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, game->shadowMapping->depthMap);
	this->deferredPBR->setMat4(UNIFORM_MATRIX_LIGHTSPACE, game->shadowMapping->lightSpace);

	// render
	for (unsigned int i = 0; i < this->bufferG.textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->bufferG.textures[i]);
	}

	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


void RenderPass::renderBounding(std::vector<MeshRenderer*>* renderQueue, GameObject* camera) {
	Game* game = Game::getInstance();

	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->bufferG.fbo);
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
	glBindTexture(GL_TEXTURE_2D, this->bufferG.textures[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


/*------------------------------------------------------------------------------
< set active light probe >
------------------------------------------------------------------------------*/
void RenderPass::setActiveLightProbe(LightProbe* lightProbe) {
	this->deferredPBR->use();
	// ibl
	this->deferredPBR->setInt(UNIFORM_TEX_IRRADIANCE, 10);
	this->deferredPBR->setInt(UNIFORM_TEX_PREFILTER, 11);
	this->deferredPBR->setInt(UNIFORM_TEX_BRDFLUT, 12);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lightProbe->irradiance);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lightProbe->prefilter);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, lightProbe->brdf);
}


/*------------------------------------------------------------------------------
< create framebuffer >
------------------------------------------------------------------------------*/
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
		frameBuffer.textures.push_back(colorBuffers[i]);
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
