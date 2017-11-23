
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
FxTorchFire::FxTorchFire() {

}

FxTorchFire* FxTorchFire::getInstance() {
	static FxTorchFire* instance;
	return instance;
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
FxTorchFire::~FxTorchFire() {

}


/*------------------------------------------------------------------------------
< create prefab >
------------------------------------------------------------------------------*/
void FxTorchFire::createPrefab(std::string name, glm::mat4 model, Scene* scene) {
	Game* game = Game::getInstance();
	// create game object
	GameObject* prefab = new GameObject();
	Transform* prefabTransform = prefab->addComponent<Transform>();
	prefabTransform->position = model * glm::vec4(0, 0, 0, 1);
	// fire emitter
	Emitter* fireEmitter = prefab->addComponent<Emitter>();
	fireEmitter->material = game->resources->getMaterial("fx_fire");
	fireEmitter->animation = true;
	fireEmitter->divideX = 4;
	fireEmitter->divideY = 4;
	fireEmitter->sampleTime = 0.1;
	fireEmitter->maxParticles = 26;
	fireEmitter->spawnTime = 0.01;
	fireEmitter->minLifeTime = 1;
	fireEmitter->maxLifeTime = 1.5;
	fireEmitter->initScale = 10 * GLOBAL_SCALE;
	fireEmitter->velocityHorizonalRange = 0.05;
	fireEmitter->velocityVerticalRange = 0.1;
	fireEmitter->gravity = -0.5;
	fireEmitter->createParticles((name + "fire").c_str(), scene);
	// add gameobject to scene
	scene->addGameObject(name.c_str(), prefab);
}
