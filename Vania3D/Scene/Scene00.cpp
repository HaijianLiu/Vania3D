
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
	// this->lightColors[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	// this->lightColors[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	// this->lightColors[2] = glm::vec3(0.0f, 0.0f, 0.0f);
	// this->lightColors[3] = glm::vec3(0.0f, 0.0f, 0.0f);

	// initialize static shader uniforms before rendering
	game->resources->getShader("deferredPBRforUEmask")->use();
		// camera
		game->resources->getShader("deferredPBRforUEmask")->setMat4("projection", this->camera->getMatrixProjection());
		// transform
		glm::mat4 model = glm::mat4();
		model = glm::rotate(-PI/2, glm::vec3(1,0,0)) * model;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
		// texture
		game->resources->getShader("deferredPBRforUEmask")->setMat4("model", model);
		game->resources->getShader("deferredPBRforUEmask")->setInt("albedoMap", 0);
		game->resources->getShader("deferredPBRforUEmask")->setInt("normalMap", 1);
		game->resources->getShader("deferredPBRforUEmask")->setInt("maskMap", 2);
		// game->resources->getShader("deferredPBRforUEmask")->setInt("roughnessMap", 3);
		// game->resources->getShader("deferredPBRforUEmask")->setInt("aoMap", 4);

	// IBL
	game->renderPass->setActiveLightProbe(game->resources->getLightProbe("hdr"));

	game->resources->getShader("renderPass")->use();
	// camera
	game->resources->getShader("renderPass")->setMat4("projection", this->camera->getMatrixProjection());
	// lights
	for (unsigned int i = 0; i < sizeof(this->lightPositions) / sizeof(this->lightPositions[0]); ++i) {
		game->resources->getShader("renderPass")->setVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
		game->resources->getShader("renderPass")->setVec3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
	}
	// Enable alpha channel
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

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


// object
	// shader
	game->resources->getShader("deferredPBRforUEmask")->use();
		// camera
		game->resources->getShader("deferredPBRforUEmask")->setMat4("view", this->camera->getMatrixView());
		// transform
		// model = glm::scale(glm::vec3(0.1f));
		// model = glm::translate(glm::vec3(0.0, 0.0, 0.0));
		// game->resources->getShader("deferredPBRforUEmask")->setMat4("model", model);
		// texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("Maw_J_Laygo_albedo")->id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("Maw_J_Laygo_normal")->id);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("Maw_J_Laygo_mask")->id);
		// glActiveTexture(GL_TEXTURE3);
		// glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("WPN_AKM_roughness")->id);
		// glActiveTexture(GL_TEXTURE4);
		// glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("WPN_AKM_ao")->id);
	game->resources->getModel("Maw_J_Laygo")->draw();


// renderPass
	game->resources->getShader("renderPass")->use();
	game->resources->getShader("renderPass")->setVec3("cameraPos", this->camera->getPosition());



	this->camera->updateInput(game->window->window, this->deltaTime);
}
