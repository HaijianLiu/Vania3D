
#include "Engine.hpp"


/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
RenderPipeline::RenderPipeline() {

}


/*------------------------------------------------------------------------------
< Desstructor >
------------------------------------------------------------------------------*/
RenderPipeline::~RenderPipeline() {
	deleteVector(this->renderPasses);
}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void RenderPipeline::start() {
	// init engine members
	this->game = Game::getInstance();
	this->quad = game->resources->quad;

	// hdr
	this->lightProbe = this->game->resources->getLightProbe("hdr");
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->lightProbe->irradiance);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->lightProbe->prefilter);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, this->lightProbe->brdf);
	
	// default lut pass
	this->currentLut = this->game->resources->getTexture("clut_default_a")->id;
	this->lutShader = this->game->resources->getShader("lut_pass");
	this->lutShader->use();
	this->lutShader->setInt("combinePass", 0);
	this->lutShader->setInt("lut", 1);
}


/*------------------------------------------------------------------------------
< add RenderPass >
------------------------------------------------------------------------------*/
void RenderPipeline::addRenderPass(RenderPass* renderPass) {
	renderPass->start();
	this->renderPasses.push_back(renderPass);
}


/*------------------------------------------------------------------------------
< render >
------------------------------------------------------------------------------*/
void RenderPipeline::render(RenderLayer* renderLayer, RenderLayer* fxLayer, std::vector<PointLight*>* pointLights, GameObject* camera, Scene* scene) {

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
	this->renderPasses[2]->shader->updateSystemUniforms(scene);
	this->quad->draw();

	// lighting pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderPasses[3]->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->renderPasses[3]->shader->use();
	this->renderPasses[3]->shader->updateSystemUniforms(scene);
	this->quad->draw();

	// shadow pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderPasses[4]->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->renderPasses[4]->shader->use();
	this->renderPasses[4]->shader->updateSystemUniforms(scene);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, game->shadowMapping->depthMap);
	this->quad->draw();

	// ssao pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderPasses[5]->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->renderPasses[5]->shader->use();
	this->renderPasses[5]->shader->updateSystemUniforms(scene);
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

