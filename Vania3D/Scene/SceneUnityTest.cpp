
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
SceneUnityTest::SceneUnityTest() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
SceneUnityTest::~SceneUnityTest() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void SceneUnityTest::start() {
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

	// light
	GameObject* light[4];
	for (int i = 0; i < 4; i++) {
		light[i] = new GameObject();
		Transform* lightTransform = light[i]->addComponent<Transform>();
		lightTransform->modelScale = glm::vec3(0.1);
		light[i]->addComponent<PointLight>();
		// for test
		MeshRenderer* lightMeshRenderer = light[i]->addComponent<MeshRenderer>();
		lightMeshRenderer->addModel(game->resources->getModel("sphere"));
		lightMeshRenderer->addMaterial(game->resources->getMaterial("simple"));
		lightMeshRenderer->camera = camera;
		// ...
		this->addLight(light[i]);
	}
	light[0]->getComponent<Transform>()->position = glm::vec3( 10.0f,  10.0f,  10.0f);
	light[1]->getComponent<Transform>()->position = glm::vec3( 10.0f,  10.0f, -10.0f);
	light[2]->getComponent<Transform>()->position = glm::vec3(-10.0f,  10.0f,  10.0f);
	light[3]->getComponent<Transform>()->position = glm::vec3(-10.0f,  10.0f, -10.0f);
	light[0]->getComponent<PointLight>()->color = glm::vec3(100.0f, 100.0f, 100.0f);
	light[1]->getComponent<PointLight>()->color = glm::vec3(100.0f, 0.0f, 0.0f);
	light[2]->getComponent<PointLight>()->color = glm::vec3(0.0f, 100.0f, 0.0f);
	light[3]->getComponent<PointLight>()->color = glm::vec3(0.0f, 0.0f, 100.0f);


	// IBL
	game->renderPass->setActiveLightProbe(game->resources->getLightProbe("hdr"));

	// Enable alpha channel after generate prefilter map
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void SceneUnityTest::update() {
	// show light spere, for test
	for (unsigned int i = 0; i < this->lights.size(); i ++) {
		this->lights[i]->getComponent<Transform>()->update();
		this->lights[i]->getComponent<MeshRenderer>()->update();
	}
}
