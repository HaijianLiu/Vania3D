
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Scene::Scene() {
	this->game = Game::getInstance();
	this->renderLayer = new RenderLayer();
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Scene::~Scene() {
	deleteVector(this->gameObjects);
	delete this->renderLayer;
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
		if (meshRenderer) {
			this->renderLayer->add(this->gameObjects[i]);
			this->renderQueue.push_back(meshRenderer);
		}
		if (this->gameObjects[i]->getComponent<PointLight>())
			this->pointLights.push_back(this->gameObjects[i]);
	}
	this->started = true;
}


/*------------------------------------------------------------------------------
 < update scene > for scene manager
------------------------------------------------------------------------------*/
void Scene::updateScene() {
	// user scene update
	this->update();
	// game objects update
	for (unsigned int i = 0; i < this->gameObjects.size(); i++)
		this->gameObjects[i]->update();
	// frustum culling
	FrustumCulling* frustumCulling = this->mainCamera->getComponent<FrustumCulling>();
	if (frustumCulling)
		for (unsigned int i = 0; i < this->renderQueue.size(); i++)
			frustumCulling->cullingSphere(this->renderQueue.at(i));
	// shadow mapping
	this->game->shadowMapping->render(&this->shadowQueue);
	// final render
	this->game->renderPass->render(this->renderLayer, &this->pointLights, this->mainCamera);
//	this->game->renderPass->renderBounding(&this->renderQueue, this->mainCamera);
}


/*------------------------------------------------------------------------------
< add gameobject >
------------------------------------------------------------------------------*/
void Scene::addGameObject(const char* name, GameObject* gameObject) {
	gameObject->scene = this;
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
