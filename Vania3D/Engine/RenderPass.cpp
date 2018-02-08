
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
RenderPass::RenderPass(std::string name) {
	this->name = name;
}


/*------------------------------------------------------------------------------
< Desstructor >
------------------------------------------------------------------------------*/
RenderPass::~RenderPass() {
	
}


/*------------------------------------------------------------------------------
< add color attachment >
------------------------------------------------------------------------------*/
void RenderPass::addColorAttachment(GLint internalFormat) {
	this->colorAttachments.push_back(internalFormat);
}


/*------------------------------------------------------------------------------
< add depth attachment >
------------------------------------------------------------------------------*/
void RenderPass::addDepthAttachment(GLenum internalFormat) {
	this->depthAttachments.push_back(internalFormat);
}


/*------------------------------------------------------------------------------
< add shader >
------------------------------------------------------------------------------*/
void RenderPass::addShader(Shader* shader) {
	this->shader = shader;
}


/*------------------------------------------------------------------------------
< add dynamic texture attachment >
------------------------------------------------------------------------------*/
void RenderPass::addDynamicTextureAttachment(GLenum unit, GLenum target, GLuint texture) {
	TextureAttachment textureAttachment;
	textureAttachment.unit = unit;
	textureAttachment.target = target;
	textureAttachment.texture = texture;
	this->dynamicTextureAttachments.push_back(textureAttachment);
}


/*------------------------------------------------------------------------------
< add dynamic texture attachment >
------------------------------------------------------------------------------*/
unsigned int RenderPass::getTexture(unsigned int index) {
	return this->frameBuffer.textures[index];
}


/*------------------------------------------------------------------------------
< start render pass >
------------------------------------------------------------------------------*/
void RenderPass::start() {
	glGenFramebuffers(1, &this->frameBuffer.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer.fbo);
	
	// color attachment
	for (unsigned int i = 0; i < this->colorAttachments.size(); i++) {
		unsigned int textureID = createColorAttachment(GL_COLOR_ATTACHMENT0 + i, this->colorAttachments[i]);
		this->frameBuffer.textures.push_back(textureID);
	}
	drawBuffers(this->colorAttachments.size());
	
	// depth attachment
	for (unsigned int i = 0; i < this->depthAttachments.size(); i++) {
		createDepthAttachment(this->depthAttachments[0]); // only one depth // 
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/*------------------------------------------------------------------------------
< update render pass >
------------------------------------------------------------------------------*/
void RenderPass::update() {
	// update dynamic texture attachments
	for (unsigned int i = 0; i < this->dynamicTextureAttachments.size(); i++) {
		glActiveTexture(this->dynamicTextureAttachments[i].unit);
		glBindTexture(this->dynamicTextureAttachments[i].target, this->dynamicTextureAttachments[i].texture);
	}
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
	else if (internalFormat == GL_RED)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, game->window->screenWidth, game->window->screenHeight, 0, GL_RED, GL_FLOAT, NULL);
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
