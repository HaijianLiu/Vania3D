
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
	} else {
		this->scenes[this->currentScene]->start();
		this->scenes[this->currentScene]->started = true;
	}
}


/*------------------------------------------------------------------------------
< add >
------------------------------------------------------------------------------*/
void SceneManager::add(const char* name, Scene* scene) {
	this->scenes.insert(std::make_pair(name, scene));
}


/*------------------------------------------------------------------------------
< set active scene >
------------------------------------------------------------------------------*/
void SceneManager::setActiveScene(const char* name) {
	this->currentScene = name;
}
