
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Scene00::Scene00() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Scene00::~Scene00() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void Scene00::start() {
	Game* game = Game::getInstance();
	this->camera = new Camera();

	this->lightPositions[0] = glm::vec3(-10.0f,  10.0f, 20.0f);
	this->lightPositions[1] = glm::vec3( 10.0f,  10.0f, 20.0f);
	this->lightPositions[2] = glm::vec3(-10.0f, -10.0f, 20.0f);
	this->lightPositions[3] = glm::vec3( 10.0f, -10.0f, 20.0f);
	this->lightColors[0] = glm::vec3(100.0f, 100.0f, 100.0f);
	this->lightColors[1] = glm::vec3(100.0f, 100.0f, 100.0f);
	this->lightColors[2] = glm::vec3(100.0f, 100.0f, 100.0f);
	this->lightColors[3] = glm::vec3(100.0f, 100.0f, 100.0f);

	// initialize static shader uniforms before rendering
	game->resources->getShader("deferredPBR")->use();
		// matrix
		glm::mat4 projection = this->camera->getMatrixProjection();
		game->resources->getShader("deferredPBR")->setMat4("projection", projection);
		glm::mat4 model = glm::mat4();
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
		// texture
		game->resources->getShader("deferredPBR")->setMat4("model", model);
		game->resources->getShader("deferredPBR")->setInt("albedoMap", 0);
		game->resources->getShader("deferredPBR")->setInt("normalMap", 1);
		game->resources->getShader("deferredPBR")->setInt("metallicMap", 2);
		game->resources->getShader("deferredPBR")->setInt("roughnessMap", 3);
		game->resources->getShader("deferredPBR")->setInt("aoMap", 4);


	game->renderPass->setActiveLightProbe(game->resources->getLightProbe("hdr"));

	game->resources->getShader("renderPass")->use();
	// matrix
	game->resources->getShader("renderPass")->setMat4("projection", projection);
	// IBL
	// lights
	for (unsigned int i = 0; i < sizeof(this->lightPositions) / sizeof(this->lightPositions[0]); ++i) {
		game->resources->getShader("renderPass")->setVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
		game->resources->getShader("renderPass")->setVec3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
	}
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Scene00::update() {
	Game* game = Game::getInstance();

	// per-frame time logic
	float currentFrame = glfwGetTime();
	this->deltaTime = currentFrame - this->lastFrame;
	this->lastFrame = currentFrame;

	glm::mat4 model = glm::mat4();

	glm::mat4 view = this->camera->getMatrixView();
	game->resources->getShader("deferredPBR")->use();
		game->resources->getShader("deferredPBR")->setMat4("view", view);
		game->resources->getShader("deferredPBR")->setVec3("camPos", this->camera->getPosition());
		model = glm::scale(glm::vec3(0.8f));
		model = glm::translate(glm::vec3(0.0, 0.0, 0.0));
		game->resources->getShader("deferredPBR")->setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("WPN_AKM_albedo")->id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("WPN_AKM_normal")->id);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("WPN_AKM_metallic")->id);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("WPN_AKM_roughness")->id);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("WPN_AKM_ao")->id);
	game->resources->getModel("WPN_AKM")->draw();

	game->resources->getShader("renderPass")->use();
	game->resources->getShader("renderPass")->setVec3("cameraPos", this->camera->getPosition());

	this->camera->updateInput(game->window->window, this->deltaTime);
}
