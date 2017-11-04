
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Scene::Scene() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Scene::~Scene() {
	deleteVector(this->gameObjects);
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Scene::update() {

}


/*------------------------------------------------------------------------------
< add game object >
------------------------------------------------------------------------------*/
void Scene::addGameObject(const char* name, GameObject* gameObject) {
	this->gameObjects.push_back(gameObject);
	this->gameObjectsMapping.insert(std::make_pair(name, this->index));
	this->index ++;
}

GameObject* Scene::getGameObject(const char* name) {
	auto it = this->gameObjectsMapping.find(name);
	if (it != this->gameObjectsMapping.end())
		return this->gameObjects[it->second];
	else
		return nullptr;
}
