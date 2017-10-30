
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Game::Game() {
	this->window = new Window("vania", SCREEN_WIDTH, SCREEN_HEIGHT);
	this->resources = Resources::getInstance();
	this->sceneManager = new SceneManager();
	this->renderPass = new RenderPass();
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
}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void Game::start() {
	this->resources->start();
	this->renderPass->init(this->resources->getShader("renderPass"), 5);
	this->input = Input::getInstance();
	// create scene ????
	this->sceneManager->add("Scene00", new Scene00());
	this->sceneManager->setActiveScene("Scene00");
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Game::update() {
	this->renderPass->begin();
	this->sceneManager->update();
	this->renderPass->end();
	this->renderPass->render();
}
