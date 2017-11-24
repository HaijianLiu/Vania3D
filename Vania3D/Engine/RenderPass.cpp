
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
< start >
------------------------------------------------------------------------------*/
void RenderPass::start() {
	Game* game = Game::getInstance();
	this->quad = game->resources->quad;

	// deferred pass
	glGenFramebuffers(1, &this->deferredPass.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->deferredPass.fbo);
	this->deferredPass.textures.push_back(createColorAttachment(GL_COLOR_ATTACHMENT0, GL_RGB));
	this->deferredPass.textures.push_back(createColorAttachment(GL_COLOR_ATTACHMENT1, GL_RGB16F));
	this->deferredPass.textures.push_back(createColorAttachment(GL_COLOR_ATTACHMENT2, GL_RGB));
	this->deferredPass.textures.push_back(createColorAttachment(GL_COLOR_ATTACHMENT3, GL_RGB16F));
	drawBuffers(4);
	createDepthAttachment(GL_DEPTH_COMPONENT24);
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	// fx pass
	glGenFramebuffers(1, &this->fxPass.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fxPass.fbo);
	this->fxPass.textures.push_back(createColorAttachment(GL_COLOR_ATTACHMENT0, GL_RGB));
	drawBuffers(1);
	createDepthAttachment(GL_DEPTH_COMPONENT24);
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	// lighting pass
	glGenFramebuffers(1, &this->lightingPass.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->lightingPass.fbo);
	this->lightingPass.textures.push_back(createColorAttachment(GL_COLOR_ATTACHMENT0, GL_RGB));
	drawBuffers(1);
	createDepthAttachment(GL_DEPTH_COMPONENT24);
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	this->lightingShader = game->resources->getShader("lighting_pass");
	this->lightingShader->use();
	this->lightingShader->setInt("albedoPass", 0);
	this->lightingShader->setInt("normalPass", 1);
	this->lightingShader->setInt("mrcPass", 2);
	this->lightingShader->setInt("positionPass", 3);

	// final shader
	LightProbe* lightProbe = game->resources->getLightProbe("hdr");
	this->finalShader = game->resources->getShader("renderpass_deferred_pbr");
	this->finalShader->use();
	this->finalShader->setInt("albedoPass", 0);
	this->finalShader->setInt("normalPass", 1);
	this->finalShader->setInt("mrcPass", 2);
	this->finalShader->setInt("positionPass", 3);
	this->finalShader->setInt(UNIFORM_TEX_IRRADIANCE, 10);
	this->finalShader->setInt(UNIFORM_TEX_PREFILTER, 11);
	this->finalShader->setInt(UNIFORM_TEX_BRDFLUT, 12);
	this->finalShader->setInt(UNIFORM_TEX_SHADOW, 13);
	this->finalShader->setInt("fxPass", 14);
	this->finalShader->setInt("lightingPass", 15);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lightProbe->irradiance);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lightProbe->prefilter);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, lightProbe->brdf);
}


/*------------------------------------------------------------------------------
< render >
------------------------------------------------------------------------------*/
void RenderPass::render(RenderLayer* renderLayer, RenderLayer* fxLayer, std::vector<PointLight*>* pointLights, GameObject* camera) {
	Game* game = Game::getInstance();
	int width = game->window->screenWidth;
	int height = game->window->screenHeight;

	// deferred pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->deferredPass.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderLayer->render(camera);

	// fx pass
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fxPass.fbo);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	fxLayer->render(camera);
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// lighting pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->lightingPass.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->lightingShader->use();
	for (unsigned int i = 0; i < this->deferredPass.textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->deferredPass.textures[i]);
	}
	this->lightingShader->setVec3(UNIFORM_VEC3_CAMERA_POSITION, camera->transform->position);
	int lightSize = 0;
	for (unsigned int i = 0; i < pointLights->size(); i++) {
		if (!pointLights->at(i)->culling) {
			this->lightingShader->setVec3(("lightColor[" + std::to_string(lightSize) + "]").c_str(), pointLights->at(i)->color * pointLights->at(i)->intensity);
			this->lightingShader->setVec3(("lightPosition[" + std::to_string(lightSize) + "]").c_str(), pointLights->at(i)->gameObject->transform->position);
			this->lightingShader->setFloat(("lightRadius[" + std::to_string(lightSize) + "]").c_str(), pointLights->at(i)->radius);
			lightSize++;
		}
	}
	this->lightingShader->setInt("lightSize", lightSize);
	game->resources->quad->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// final pass
	this->finalShader->use();
	this->finalShader->setVec3(UNIFORM_VEC3_CAMERA_POSITION, camera->transform->position);
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, game->shadowMapping->depthMap);
	this->finalShader->setMat4(UNIFORM_MATRIX_LIGHTSPACE, game->shadowMapping->lightSpace);
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, this->fxPass.textures[0]);
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, this->lightingPass.textures[0]);
	for (unsigned int i = 0; i < this->deferredPass.textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->deferredPass.textures[i]);
	}
	this->quad->draw();
}


/*------------------------------------------------------------------------------
< render bounding box >
------------------------------------------------------------------------------*/
void RenderPass::renderBounding(std::vector<MeshRenderer*>* renderQueue, GameObject* camera) {
	Game* game = Game::getInstance();

	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->deferredPass.fbo);
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
	glBindTexture(GL_TEXTURE_2D, this->deferredPass.textures[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	this->quad->draw();
}


/*------------------------------------------------------------------------------
< create color attachment >
------------------------------------------------------------------------------*/
unsigned int RenderPass::createColorAttachment(GLenum attachment, GLint internalFormat) {
	Game* game = Game::getInstance();

	// create attachment
	unsigned int colorAttachment;
	glGenTextures(1, &colorAttachment);
	glBindTexture(GL_TEXTURE_2D, colorAttachment);

	if (internalFormat == GL_RGB)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, game->window->screenWidth, game->window->screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	else if (internalFormat == GL_RGB16F)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, game->window->screenWidth, game->window->screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	else if (internalFormat == GL_RGB32F)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, game->window->screenWidth, game->window->screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	else return -1;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, colorAttachment, 0);

	return colorAttachment;
}


/*------------------------------------------------------------------------------
< create depth attachment >
------------------------------------------------------------------------------*/
void RenderPass::createDepthAttachment(GLenum internalformat) {
	Game* game = Game::getInstance();

	unsigned int depthRBO;
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, internalformat, game->window->screenWidth, game->window->screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
}


/*------------------------------------------------------------------------------
< set draw buffers >
------------------------------------------------------------------------------*/
void RenderPass::drawBuffers(GLsizei n) {
	unsigned int attachments[n];
	for (unsigned int i = 0; i < n; i++)
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	glDrawBuffers(n, attachments);
}
