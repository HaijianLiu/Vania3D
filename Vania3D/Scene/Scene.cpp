
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Scene::Scene() {
	this->game = Game::getInstance();
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
 < start scene > for scene manager
------------------------------------------------------------------------------*/
void Scene::startScene() {
	this->start();
	for (unsigned int i = 0; i < this->gameObjects.size(); i++) {
		this->gameObjects[i]->start();
        MeshRenderer* meshRenderer = this->gameObjects[i]->getComponent<MeshRenderer>();
		if (meshRenderer && meshRenderer->castShadow)
			this->shadowQueue.push_back(meshRenderer);
		if (meshRenderer && meshRenderer->render)
			this->renderQueue.push_back(meshRenderer);
		if (this->gameObjects[i]->getComponent<PointLight>()) {
			this->pointLights.push_back(this->gameObjects[i]);
		}
	}
	this->started = true;
}


/*------------------------------------------------------------------------------
 < update scene > for scene manager
------------------------------------------------------------------------------*/
void Scene::updateScene() {
	this->update();
	for (unsigned int i = 0; i < this->gameObjects.size(); i++)
		this->gameObjects[i]->update();
	this->game->shadowMapping->render(&this->shadowQueue);
	this->game->renderPass->render(&this->renderQueue, &this->pointLights, this->mainCamera);
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
< get gameobject >
------------------------------------------------------------------------------*/
GameObject* Scene::getGameObject(const char* name) {
	auto it = this->gameObjectsMapping.find(name);
	if (it != this->gameObjectsMapping.end())
		return this->gameObjects[it->second];
	else
		return nullptr;
}
