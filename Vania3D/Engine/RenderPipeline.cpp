
#include "Engine.hpp"

std::vector<glm::vec3> genSSAOKernel(unsigned int kernelSize) {
	
	std::vector<glm::vec3> ssaoKernel;
	
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;
	
	for (unsigned int i = 0; i < kernelSize; ++i) {
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / kernelSize;
		// scale samples s.t. they're more aligned to center of kernel
		scale = 0.1f + (1.0f - 0.1f) * scale * scale;
		sample *= scale;
		ssaoKernel.push_back(sample);
	}
	return ssaoKernel;
}



/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
RenderPipeline::RenderPipeline() {

}


/*------------------------------------------------------------------------------
< Desstructor >
------------------------------------------------------------------------------*/
RenderPipeline::~RenderPipeline() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void RenderPipeline::start() {
	// init engine members
	this->game = Game::getInstance();
	this->quad = game->resources->quad;
	
	// deferred pass
	RenderPass* deferredPass = new RenderPass("deferredPass");
	deferredPass->addColorAttachment(GL_RGB);
	deferredPass->addColorAttachment(GL_RGB16F);
	deferredPass->addColorAttachment(GL_RGB);
	deferredPass->addColorAttachment(GL_RGB16F);
	deferredPass->addDepthAttachment(GL_DEPTH_COMPONENT24);
	deferredPass->start();
	this->addRenderPass(deferredPass); // 0
	
	// fx pass
	RenderPass* fxPass = new RenderPass("fxPass");
	fxPass->addColorAttachment(GL_RGB);
	fxPass->addDepthAttachment(GL_DEPTH_COMPONENT24);
	fxPass->start();
	this->addRenderPass(fxPass); // 1

	// ambient pass
	RenderPass* ambientPass = new RenderPass("ambientPass");
	ambientPass->shader = this->game->resources->getShader("ambient_pass");
	ambientPass->shader->use();
	ambientPass->shader->setInt("albedoPass", 0);
	ambientPass->shader->setInt("normalPass", 1);
	ambientPass->shader->setInt("mrcPass", 2);
	ambientPass->shader->setInt("positionPass", 3);
	ambientPass->shader->setInt("irradianceMap", 10);
	ambientPass->shader->setInt("prefilterMap", 11);
	ambientPass->shader->setInt("brdfLUT", 12);
	ambientPass->addColorAttachment(GL_RGB);
	ambientPass->start();
	this->addRenderPass(ambientPass); // 2

	// lighting pass
	RenderPass* lightingPass = new RenderPass("lightingPass");
	lightingPass->shader = this->game->resources->getShader("lighting_pass");
	lightingPass->shader->use();
	lightingPass->shader->setInt("albedoPass", 0);
	lightingPass->shader->setInt("normalPass", 1);
	lightingPass->shader->setInt("mrcPass", 2);
	lightingPass->shader->setInt("positionPass", 3);
	lightingPass->addColorAttachment(GL_RGB);
	lightingPass->start();
	this->addRenderPass(lightingPass); // 3

	// shadow pass
	RenderPass* shadowPass = new RenderPass("shadowPass");
	shadowPass->shader = this->game->resources->getShader("shadow_pass");
	shadowPass->shader->use();
	shadowPass->shader->setInt("normalPass", 1);
	shadowPass->shader->setInt("positionPass", 3);
	shadowPass->shader->setInt("shadowMap", 4);
	shadowPass->addColorAttachment(GL_RED);
	shadowPass->start();
	this->addRenderPass(shadowPass); // 4

	// ssao pass
	RenderPass* ssaoPass = new RenderPass("ssaoPass");
	ssaoPass->shader = this->game->resources->getShader("ssao_pass");
	ssaoPass->shader->use();
	ssaoPass->shader->setInt("normalPass", 1);
	ssaoPass->shader->setInt("positionPass", 3);
	std::vector<glm::vec3> ssaoKernel = genSSAOKernel(4);
	for (unsigned int i = 0; i < ssaoKernel.size(); ++i)
		ssaoPass->shader->setVec3(("samples[" + std::to_string(i) + "]").c_str(), ssaoKernel[i]);
	ssaoPass->addColorAttachment(GL_RED);
	ssaoPass->start();
	this->addRenderPass(ssaoPass); // 5

	// combine pass
	RenderPass* combinePass = new RenderPass("combinePass");
	combinePass->shader = this->game->resources->getShader("renderpass_combine");
	combinePass->shader->use();
	combinePass->shader->setInt("mrcPass", 2);
	combinePass->shader->setInt("fxPass", 4);
	combinePass->shader->setInt("ambientPass", 5);
	combinePass->shader->setInt("lightingPass", 6);
	combinePass->shader->setInt("shadowPass", 7);
	combinePass->shader->setInt("ssaoPass", 8);
	combinePass->addColorAttachment(GL_RGB);
	combinePass->start();
	this->addRenderPass(combinePass); // 6
	


	
	this->lightProbe = this->game->resources->getLightProbe("hdr");


	this->lutShader = this->game->resources->getShader("lut_pass");
	// default lut
	this->currentLut = this->game->resources->getTexture("clut_default_a")->id;
	
	// hdr
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->lightProbe->irradiance);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->lightProbe->prefilter);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, this->lightProbe->brdf);
	
	
	
	// lut pass
	this->lutShader->use();
	this->lutShader->setInt("combinePass", 0);
	this->lutShader->setInt("lut", 1);
}

void RenderPipeline::addRenderPass(RenderPass* renderPass) {
	this->renderPasses.push_back(renderPass);
}


/*------------------------------------------------------------------------------
< render >
------------------------------------------------------------------------------*/
void RenderPipeline::render(RenderLayer* renderLayer, RenderLayer* fxLayer, std::vector<PointLight*>* pointLights, GameObject* camera) {

	int width = this->game->window->screenWidth;
	int height = this->game->window->screenHeight;

	// deferred pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderPasses[0]->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderLayer->render(camera);

	// fx pass
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->renderPasses[1]->frameBuffer.fbo);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	fxLayer->render(camera);
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// active g-buffer textures
	for (unsigned int i = 0; i < this->renderPasses[0]->frameBuffer.textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->renderPasses[0]->frameBuffer.textures[i]);
	}
	
	
	
	
	

	// ambient pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderPasses[2]->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->renderPasses[2]->shader->use();
	this->renderPasses[2]->shader->setVec3("cameraPosition", camera->transform->position);
	this->quad->draw();

	// lighting pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderPasses[3]->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->renderPasses[3]->shader->use();
	this->renderPasses[3]->shader->setVec3("cameraPosition", camera->transform->position);
	int lightSize = 0;
	for (unsigned int i = 0; i < pointLights->size(); i++) {
		if (!pointLights->at(i)->culling) {
			this->renderPasses[3]->shader->setVec3(("lightColor[" + std::to_string(lightSize) + "]").c_str(), pointLights->at(i)->color * pointLights->at(i)->intensity);
			this->renderPasses[3]->shader->setVec3(("lightPosition[" + std::to_string(lightSize) + "]").c_str(), pointLights->at(i)->gameObject->transform->position);
			this->renderPasses[3]->shader->setFloat(("lightRadius[" + std::to_string(lightSize) + "]").c_str(), pointLights->at(i)->radius);
			lightSize++;
		}
	}
	this->renderPasses[3]->shader->setInt("lightSize", lightSize);
	this->quad->draw();

	// shadow pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderPasses[4]->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->renderPasses[4]->shader->use();
	this->renderPasses[4]->shader->setMat4("lightSpaceMatrix", game->shadowMapping->lightSpace);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, game->shadowMapping->depthMap);
	this->quad->draw();

	// ssao pass
	Camera* cameraComponent = camera->getComponent<Camera>();
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderPasses[5]->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->renderPasses[5]->shader->use();
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(glm::mat3(cameraComponent->view)));
	this->renderPasses[5]->shader->setMat4("normalMatrix", normalMatrix);
	cameraComponent->setUniforms(this->renderPasses[5]->shader);
	this->quad->draw();

	// combine pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderPasses[6]->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->renderPasses[6]->shader->use();
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, this->renderPasses[1]->frameBuffer.textures[0]); // fx
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, this->renderPasses[2]->frameBuffer.textures[0]); // ambient
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, this->renderPasses[3]->frameBuffer.textures[0]); // lighting
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, this->renderPasses[4]->frameBuffer.textures[0]); // shadow
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, this->renderPasses[5]->frameBuffer.textures[0]); // ssao
	this->quad->draw();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
	
	
	
	
	
	
	// lut pass
	this->lutShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->renderPasses[6]->frameBuffer.textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->currentLut);
	
	this->quad->draw();
}


/*------------------------------------------------------------------------------
< render bounding box >
------------------------------------------------------------------------------*/
void RenderPipeline::renderBounding(std::vector<MeshRenderer*>* renderQueue, GameObject* camera) {
	Game* game = Game::getInstance();

	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderPasses[0]->frameBuffer.fbo);
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
	glBindTexture(GL_TEXTURE_2D, this->renderPasses[0]->frameBuffer.textures[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	this->quad->draw();
}

