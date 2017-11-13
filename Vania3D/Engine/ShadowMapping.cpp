
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
ShadowMapping::ShadowMapping() {
	this->lightPositionOffset = glm::vec3(4, 4, -4);
	this->range = 4;
	this->nearPlane = 0;
	this->farPlane = 12;
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
ShadowMapping::~ShadowMapping() {

}


/*------------------------------------------------------------------------------
 < init framebuffer and shadow map >
------------------------------------------------------------------------------*/
void ShadowMapping::init(Shader* shader, unsigned int size) {
	// save shadow mapping shader
	this->shader = shader;
	// map size and screen size
	this->size = size;
	this->retina = Game::getInstance()->window->retina;

	// configure depth map fbo
	glGenFramebuffers(1, &this->fbo);
	// create depth texture
	glGenTextures(1, &this->depthMap);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as fbo depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// light projection
	this->projection = glm::ortho(-this->range, this->range, -this->range, this->range, this->nearPlane, this->farPlane);
}


/*------------------------------------------------------------------------------
 < render >
------------------------------------------------------------------------------*/
void ShadowMapping::render(std::vector<MeshRenderer*>* shadowQueue) {
	Game* game = Game::getInstance();

	// bind framebuffer and view port
	glViewport(0, 0, this->size, this->size);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	// bind shader
	this->shader->use();

	// light space matrix
	Transform* targetTransform = this->target->getComponent<Transform>();
	this->view = glm::lookAt(this->lightPositionOffset + targetTransform->position, targetTransform->position, game->worldUp);
	this->lightSpace = this->projection * this->view;
	this->shader->setMat4(UNIFORM_MATRIX_LIGHTSPACE, this->lightSpace);
	// draw
	for (unsigned int i = 0; i < shadowQueue->size(); i++)
		shadowQueue->at(i)->renderShadow();

	// reset framebuffer and view port
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH * this->retina, SCREEN_HEIGHT * this->retina);
}
