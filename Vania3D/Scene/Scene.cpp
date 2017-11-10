
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
	deleteVector(this->lights);
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
	for (unsigned int i = 0; i < this->gameObjects.size(); i++)
		this->gameObjects[i]->start();
	this->started = true;
}


/*------------------------------------------------------------------------------
 < update scene > for scene manager
------------------------------------------------------------------------------*/
void Scene::updateScene() {
	this->update();
	this->camera->update();
	this->updateShadowMapping();
	this->updateRenderPass();
}


void Scene::updateShadowMapping() {
    this->game->shadowMapping->begin();
    this->game->shadowMapping->update();
    
    //    draw
    Transform* transform = this->getGameObject("player")->getComponent<Transform>();
    game->resources->getShader("shadow_mapping_depth")->setMat4("model", transform->model);
    std::vector<glm::mat4> pose = this->getGameObject("player")->getComponent<MeshRenderer>()->model->pose;
    for (unsigned int i = 0 ; i < pose.size() ; i++)
        this->getGameObject("player")->getComponent<MeshRenderer>()->material->shader->setMat4(("bones[" + std::to_string(i) + "]").c_str(), pose[i]);
    MeshRenderer* meshRender = this->getGameObject("player")->getComponent<MeshRenderer>();
    meshRender->model->draw();
    
    
    game->shadowMapping->end();
    
    
//    renderpass
    game->renderPass->begin();

    game->resources->getShader("renderpass_deferred_pbr")->use();
    glActiveTexture(GL_TEXTURE13);
    glBindTexture(GL_TEXTURE_2D, game->shadowMapping->depthMap);
    game->resources->getShader("renderpass_deferred_pbr")->setMat4(UNIFORM_MATRIX_LIGHTSPACE, game->shadowMapping->lightSpace);
}

void Scene::updateRenderPass() {
	this->game->renderPass->begin(); // begin framebuffer
	
	for (unsigned int i = 0; i < this->gameObjects.size(); i++)
		this->gameObjects[i]->update();
	this->game->renderPass->shader->use();
	this->game->renderPass->shader->setVec3("cameraPos", this->camera->getComponent<Transform>()->position);

	// lights
	for (unsigned int i = 0; i < this->lights.size(); ++i) {
		this->game->renderPass->shader->setVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), this->lights[i]->getComponent<Transform>()->position);
		this->game->renderPass->shader->setVec3(("lightColors[" + std::to_string(i) + "]").c_str(), this->lights[i]->getComponent<PointLight>()->color);
	}
	
	this->game->renderPass->end();
	this->game->renderPass->render();
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
< add camera >
------------------------------------------------------------------------------*/
void Scene::addCamera(GameObject* camera) {
	this->camera = camera;
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
