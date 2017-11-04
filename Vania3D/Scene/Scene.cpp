
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
	deleteVector(this->lights);
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Scene::update() {

}


/*------------------------------------------------------------------------------
< add gameobject >
------------------------------------------------------------------------------*/
void Scene::addGameObject(const char* name, GameObject* gameObject) {
	this->gameObjects.push_back(gameObject);
	this->gameObjectsMapping.insert(std::make_pair(name, this->index));
	this->index ++;
}


/*------------------------------------------------------------------------------
< add light >
------------------------------------------------------------------------------*/
void Scene::addLight(GameObject* light) {
	this->lights.push_back(light);
}


/*------------------------------------------------------------------------------
< get gameobject >
------------------------------------------------------------------------------*/
GameObject* Scene::getGameObject(const char* name) {
	auto it = this->gameObjectsMapping.find(name);
	if (it != this->gameObjectsMapping.end())
		return this->gameObjects[it->second];
	else
		return nullptr;
}
