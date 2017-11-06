
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Scene01::Scene01() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Scene01::~Scene01() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void Scene01::start() {
	Game* game = Game::getInstance();

	// camera
	GameObject* camera = new GameObject();
	Transform* cameraTransform = camera->addComponent<Transform>();
	cameraTransform->position = glm::vec3(0.0,10.0,20.0);
	camera->addComponent<Camera>();

	// player
	GameObject* player = new GameObject();
	Transform* playerTransform = player->addComponent<Transform>();
	playerTransform->modelScale = glm::vec3(0.05);
	PlayerController* playerController = player->addComponent<PlayerController>();
	playerController->camera = camera;
	CameraController* cameraController = player->addComponent<CameraController>();
	cameraController->camera = camera;
	MeshRenderer* playerMeshRenderer = player->addComponent<MeshRenderer>();
	playerMeshRenderer->addModel(game->resources->getModel("player"));
	playerMeshRenderer->addMaterial(game->resources->getMaterial("player"));
	playerMeshRenderer->addLightProbe(game->resources->getLightProbe("hdr"));
	playerMeshRenderer->camera = camera;
	this->addGameObject("player", player);

	// camera target
	GameObject* cameraTarget = new GameObject();
	Transform* cameraTargetTransform = cameraTarget->addComponent<Transform>();
	Offset* cameraTargetOffset = cameraTarget->addComponent<Offset>();
	cameraTargetOffset->parent = playerTransform;
	cameraTargetOffset->offsetPosition = glm::vec3(0, 5, 0);
	this->addGameObject("cameraTarget", cameraTarget);

	// camera
	camera->getComponent<Camera>()->target = cameraTargetTransform;
	camera->getComponent<Camera>()->offsetFromTarget = cameraTransform->position - camera->getComponent<Camera>()->target->position;
	this->addCamera(camera);



	// IBL
	game->renderPass->setActiveLightProbe(game->resources->getLightProbe("hdr"));

	// Enable alpha channel after generate prefilter map
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Scene01::update() {

}
