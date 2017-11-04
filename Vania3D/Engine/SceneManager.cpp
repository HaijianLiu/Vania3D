
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
SceneManager::SceneManager() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
SceneManager::~SceneManager() {
	deleteMap(this->scenes);
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void SceneManager::update() {
	if (this->scenes[this->currentScene]->started) {
		this->scenes[this->currentScene]->update();
		for (unsigned int i = 0; i < this->scenes[this->currentScene]->gameObjects.size(); i++) {
			this->scenes[this->currentScene]->gameObjects[i]->update();
		}
		this->scenes[this->currentScene]->updateRenderPass();
	} else {
		this->scenes[this->currentScene]->start();
		for (unsigned int i = 0; i < this->scenes[this->currentScene]->gameObjects.size(); i++) {
			this->scenes[this->currentScene]->gameObjects[i]->start();
		}
		this->scenes[this->currentScene]->started = true;
	}
}


/*------------------------------------------------------------------------------
< add >
------------------------------------------------------------------------------*/
void SceneManager::addScene(const char* name, Scene* scene) {
	this->scenes.insert(std::make_pair(name, scene));
}


/*------------------------------------------------------------------------------
< set active scene >
------------------------------------------------------------------------------*/
void SceneManager::setActiveScene(const char* name) {
	this->currentScene = name;
}
