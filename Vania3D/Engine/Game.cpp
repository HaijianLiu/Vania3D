
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Game::Game() {
	// create engine objects
	// this->timer = new Timer();
	this->camera = new Camera();
	this->resources = new Resources();
	// this->sceneManager = new SceneManager();
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Game::~Game() {
	// delete this->timer;
	delete this->camera;
	delete this->resources;
	// delete this->sceneManager;
}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void Game::start() {
	// random seed
//    srand((unsigned)this->timer->currentTime);

	// start
	// this->timer->start();
	this->resources->start();
	// this->sceneManager->start();
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Game::update() {
	// this->timer->update();
	// this->sceneManager->update();
}


/*------------------------------------------------------------------------------
< get functions >
------------------------------------------------------------------------------*/
// std::vector<GameObject*>* Game::getGameObjects() {
// 	return &this->gameObjects;
// }
// std::vector<BoxCollider*>* Game::getColliders() {
// 	return &this->colliders;
// }
// std::vector<GameObject*> Game::copyGameObjects() {
// 	return this->gameObjects;
// }
// std::vector<BoxCollider*> Game::copyColliders() {
// 	return this->colliders;
// }
