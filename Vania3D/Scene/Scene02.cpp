
#include "Engine.hpp"


#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Scene02::Scene02() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Scene02::~Scene02() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void Scene02::start() {
	Game* game = Game::getInstance();

	// camera
	GameObject* camera = new GameObject();
	camera->staticObject = false;
	camera->addComponent<Camera>();

	// player
	GameObject* player = new GameObject();
	player->staticObject = false;
	Transform* playerTransform = player->addComponent<Transform>();
	playerTransform->modelScale = glm::vec3(GLOBAL_SCALE);
	playerTransform->position = glm::vec3(0, -0.6, 0);
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
	cameraTargetOffset->offsetPosition = glm::vec3(0, 1, 0);
	this->addGameObject("cameraTarget", cameraTarget);

	// camera
	Transform* cameraTransform = camera->addComponent<Transform>();
	cameraTransform->position = glm::vec3(0.0,2.0,4.0);
	camera->getComponent<Camera>()->target = cameraTargetTransform;
	camera->addComponent<FrustumCulling>();
	this->mainCamera = camera;
	this->addGameObject("mainCamera", camera);

	Map* map = new Map(this, "./Assets/Models/FantasyDungeon/Maps/Map.fbx");
	delete map;

	// IBL
	game->renderPass->setActiveLightProbe(game->resources->getLightProbe("hdr"));


	// Enable alpha channel after generate prefilter map
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Scene02::update() {

}
