
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
ShadowMapping::ShadowMapping() {

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
	
    
    
    
	// shader configuration (to be refactored)
	Game* game = Game::getInstance();
	game->resources->getShader("renderpass_deferred_pbr")->use();
	game->resources->getShader("renderpass_deferred_pbr")->setInt(UNIFORM_TEX_SHADOW, 13);

}


/*------------------------------------------------------------------------------
 < begin >
------------------------------------------------------------------------------*/
void ShadowMapping::begin() {
	glViewport(0, 0, this->size, this->size);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
}



/*------------------------------------------------------------------------------
 < update >
------------------------------------------------------------------------------*/
void ShadowMapping::update() {
    Game* game = Game::getInstance();
    // light view
    this->view = glm::lookAt(this->lightPosition, this->lightTarget, game->worldUp);
    // light space matrix
    this->lightSpace = this->projection * this->view;
    
    this->shader->use();
    this->shader->setMat4(UNIFORM_MATRIX_LIGHTSPACE, this->lightSpace);
}


/*------------------------------------------------------------------------------
 < end >
------------------------------------------------------------------------------*/
void ShadowMapping::end() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH * this->retina, SCREEN_HEIGHT * this->retina);
}


