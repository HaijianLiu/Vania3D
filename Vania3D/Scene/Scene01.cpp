
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
	cameraTransform->kinematic = false;
	cameraTransform->position = glm::vec3(0.0,10.0,20.0);
	camera->addComponent<Camera>();

	// player
	GameObject* player = new GameObject();
	Transform* playerTransform = player->addComponent<Transform>();
	playerTransform->modelScale = glm::vec3(0.05);
	playerTransform->kinematic = false;
	PlayerController* playerController = player->addComponent<PlayerController>();
	playerController->camera = camera;
	CameraController* cameraController = player->addComponent<CameraController>();
	cameraController->camera = camera;
	MeshRenderer* playerMeshRenderer = player->addComponent<MeshRenderer>();
	playerMeshRenderer->model = game->resources->getModel("player");
	playerMeshRenderer->materials.push_back(game->resources->getMaterial("player"));
	playerMeshRenderer->lightProbe = game->resources->getLightProbe("hdr");
    playerMeshRenderer->castShadow = true;
	this->addGameObject("player", player);
	game->shadowMapping->target = player;

	// camera target
	GameObject* cameraTarget = new GameObject();
	Transform* cameraTargetTransform = cameraTarget->addComponent<Transform>();
	Offset* cameraTargetOffset = cameraTarget->addComponent<Offset>();
	cameraTargetOffset->parent = playerTransform;
	cameraTargetOffset->offsetPosition = glm::vec3(0, 5, 0);
	this->addGameObject("cameraTarget", cameraTarget);

	// camera
	camera->getComponent<Camera>()->target = cameraTargetTransform;
	this->mainCamera = camera;
	this->addGameObject("mainCamera", camera);

//	Level* level = new Level("./Assets/Models/Kowloon/map.fbx");
//	level->createGameObjects(this);
//	delete level;
    
	Map* map = new Map("./Assets/Models/InfinityBladeIceLands/Maps/test.fbx");
	map->createGameObjects(this);
	delete map;


	// light
	GameObject* light[4];
	for (int i = 0; i < 4; i++) {
		light[i] = new GameObject();
		Transform* lightTransform = light[i]->addComponent<Transform>();
		lightTransform->modelScale = glm::vec3(0.1);
		light[i]->addComponent<PointLight>();
		// for test
//		MeshRenderer* lightMeshRenderer = light[i]->addComponent<MeshRenderer>();
//		lightMeshRenderer->model = game->resources->getModel("sphere");
//		lightMeshRenderer->materials.push_back(game->resources->getMaterial("simple_flat_color"));
		this->addGameObject(("light" + std::to_string(i)).c_str(), light[i]);
	}
	light[0]->getComponent<Transform>()->position = glm::vec3( 10.0f,  10.0f,  10.0f);
	light[1]->getComponent<Transform>()->position = glm::vec3( 10.0f,  10.0f, -10.0f);
	light[2]->getComponent<Transform>()->position = glm::vec3(-10.0f,  10.0f,  10.0f);
	light[3]->getComponent<Transform>()->position = glm::vec3(-10.0f,  10.0f, -10.0f);
	light[0]->getComponent<PointLight>()->color = glm::vec3(100.0f, 100.0f, 100.0f);
	light[1]->getComponent<PointLight>()->color = glm::vec3(100.0f, 0.0f, 0.0f);
	light[2]->getComponent<PointLight>()->color = glm::vec3(0.0f, 100.0f, 0.0f);
	light[3]->getComponent<PointLight>()->color = glm::vec3(0.0f, 0.0f, 100.0f);
//	for (int i = 0; i < 4; i++)
//		light[i]->getComponent<MeshRenderer>()->materials[0]->color = light[i]->getComponent<PointLight>()->color;


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
