
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Game::Game() {

}

Game* Game::getInstance() {
	static Game* game = new Game();
	return game;
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Game::~Game() {
	delete this->window;
	delete this->resources;
	delete this->sceneManager;
	delete this->renderPass;
	delete this->shadowMapping;
	delete this->time;
	delete this->input;
}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void Game::start() {
	/* create objects */
	this->window = new Window("vania", SCREEN_WIDTH, SCREEN_HEIGHT);
	this->resources = Resources::getInstance();
	this->sceneManager = new SceneManager();
	this->renderPass = new RenderPass();
	this->shadowMapping = new ShadowMapping();
	this->time = Time::getInstance();
	this->input = Input::getInstance();

	/* start */
	// resources
	this->resources->start();
	// render pass
	this->renderPass->start();
	// shadow mapping
	this->shadowMapping->init(this->resources->getShader("shadow_mapping_depth"), 512);

	// scene manager ????
//	this->sceneManager->addScene("Scene00", new Scene00());
//	this->sceneManager->addScene("Scene01", new Scene01());
	this->sceneManager->addScene("Scene02", new Scene02());
	this->sceneManager->setActiveScene("Scene02"); // set default scene
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Game::update() {
	// keep in order
	this->time->update();
	this->input->updateJoystick();
	this->sceneManager->update();
}
