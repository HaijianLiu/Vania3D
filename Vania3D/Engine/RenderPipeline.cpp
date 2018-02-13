
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
void RenderPipeline::render(Scene* scene) {
	// window size
	int width = this->game->window->screenWidth;
	int height = this->game->window->screenHeight;

	// deferred pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderPasses[0]->frameBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->renderLayer->render(scene->mainCamera);

	// fx pass
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->renderPasses[1]->frameBuffer.fbo);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	scene->fxLayer->render(scene->mainCamera);
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// active g-buffer textures
	for (unsigned int i = 0; i < this->renderPasses[0]->frameBuffer.textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->renderPasses[0]->frameBuffer.textures[i]);
	}

	for (unsigned int i = 2; i < this->renderPasses.size(); i++)
		this->renderPasses[i]->update(scene);
}


/*------------------------------------------------------------------------------
< render bounding box > out dated
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

