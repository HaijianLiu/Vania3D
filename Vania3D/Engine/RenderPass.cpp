
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
	// get game
	Game* game = Game::getInstance();
	this->shader = shader;

	// configure (floating point) framebuffers
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

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
		this->pass.push_back(colorBuffers[i]);
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

	// Set Shader
	this->shader->use();
	std::string passes = UNIFORM_TEX_PASSES;
	for (unsigned i = 0; i < number; i++) {
		this->shader->setInt((passes + "[" + std::to_string(i) + "]").c_str(), i);
	}
}


/*------------------------------------------------------------------------------
< begin >
------------------------------------------------------------------------------*/
void RenderPass::begin() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/*------------------------------------------------------------------------------
< end >
------------------------------------------------------------------------------*/
void RenderPass::end() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/*------------------------------------------------------------------------------
< render >
------------------------------------------------------------------------------*/
void RenderPass::render() {
	this->shader->use();
	for (unsigned int i = 0; i < this->pass.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->pass[i]);
	}
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
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
