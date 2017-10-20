
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Game::Game() {
	this->window = new Window("vania", SCREEN_WIDTH, SCREEN_HEIGHT);
	this->resources = Resources::getInstance();
	this->sceneManager = new SceneManager();
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
}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void Game::start() {
	this->resources->start();

	// create scene ????
	this->sceneManager->add("Scene00", new Scene00());
	this->sceneManager->setActiveScene("Scene00");
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Game::update() {
	this->sceneManager->update();
}
