
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Scene05::Scene05() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Scene05::~Scene05() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void Scene05::start() {
	Game* game = Game::getInstance();
	Resources* resources = game->resources;

	// sky
	resources->loadShader("sky", "./Assets/Shaders/Vertex/static_2_locations_sky.vs.glsl", "./Assets/Shaders/Fragment/color_1_passes_sky.fs.glsl");
	resources->loadTexture("sky", "./Assets/Models/Basic/sky.tga");
	resources->loadModel("sky", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/Basic/sky.fbx");
	resources->createMaterial("sky", resources->getShader("sky"));
	resources->getMaterial("sky")->addTexture("albedoMap", resources->getTexture("sky"));

	GameObject* sky = new GameObject();
	Transform* skyTransform = sky->addComponent<Transform>();
	skyTransform->rotation = glm::rotation(glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
	skyTransform->scale = glm::vec3(100);
	skyTransform->updateModel();
	MeshRenderer* skyMeshRenderer = sky->addComponent<MeshRenderer>();
	skyMeshRenderer->renderLayer = RENDER_LAYER_FX;
	skyMeshRenderer->model = resources->getModel("sky");
	skyMeshRenderer->materials.push_back(resources->getMaterial("sky"));
	this->addGameObject("sky", sky);

	// camera
	GameObject* camera = new GameObject();
	camera->staticObject = false;
	camera->addComponent<Camera>();

	// player
	GameObject* player = new GameObject();
	player->staticObject = false;
	Transform* playerTransform = player->addComponent<Transform>();
	playerTransform->modelScale = glm::vec3(GLOBAL_SCALE);
	playerTransform->position = glm::vec3(0, 0, 0);
	PlayerController* playerController = player->addComponent<PlayerController>();
	playerController->camera = camera;
	CameraController* cameraController = player->addComponent<CameraController>();
	cameraController->camera = camera;
	this->addGameObject("player", player);

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

	// light
	GameObject* light[4];
	for (int i = 0; i < 4; i++) {
		light[i] = new GameObject();
		light[i]->staticObject = false;
		Transform* lightTransform = light[i]->addComponent<Transform>();
		lightTransform->modelScale = glm::vec3(5 * GLOBAL_SCALE);
		light[i]->addComponent<PointLight>();
		// for test
		MeshRenderer* lightMeshRenderer = light[i]->addComponent<MeshRenderer>();
		lightMeshRenderer->model = game->resources->getModel("sphere");
		lightMeshRenderer->materials.push_back(game->resources->getMaterial("simple"));
		this->addGameObject(("light" + std::to_string(i)).c_str(), light[i]);
	}
	light[0]->getComponent<Transform>()->position = glm::vec3(0, 2.5, -5);
	light[1]->getComponent<Transform>()->position = glm::vec3(0, 2.5, 0);
	light[2]->getComponent<Transform>()->position = glm::vec3(0, 5, -15);
	light[3]->getComponent<Transform>()->position = glm::vec3(0, 7.5, -25);
	light[0]->getComponent<PointLight>()->color = glm::vec3(1, 1, 1);
	light[0]->getComponent<PointLight>()->intensity = 5;
	light[1]->getComponent<PointLight>()->color = glm::vec3(1, 1, 1);
	light[1]->getComponent<PointLight>()->intensity = 5;
	light[2]->getComponent<PointLight>()->color = glm::vec3(1, 1, 1);
	light[2]->getComponent<PointLight>()->intensity = 5;
	light[3]->getComponent<PointLight>()->color = glm::vec3(1, 1, 1);
	light[3]->getComponent<PointLight>()->intensity = 5;
	
	
	// renderpass
	game->resources->loadShader("renderpass_combine_scene04", "./Assets/Shaders/Vertex/quad.vs.glsl", "./Assets/Shaders/RenderPassScene04/renderpass_combine.fs.glsl");
	game->resources->loadShader("ambient_pass_scene04", "./Assets/Shaders/Vertex/quad.vs.glsl",  "./Assets/Shaders/RenderPassScene04/renderpass_ambient_1_passes.fs.glsl");
	game->resources->loadShader("lighting_pass_scene04", "./Assets/Shaders/Vertex/quad.vs.glsl",  "./Assets/Shaders/RenderPassScene04/renderpass_lighting_1_passes.fs.glsl", "./Assets/Shaders/Functions/cookTorranceBRDF.fs.glsl");
	delete game->renderPass;
	game->renderPass = new RenderPass();
	game->renderPass->combineShader = game->resources->getShader("renderpass_combine_scene04");
	game->renderPass->ambientShader = game->resources->getShader("ambient_pass_scene04");
	game->renderPass->lightingShader = game->resources->getShader("lighting_pass_scene04");
	game->renderPass->start();
	
	// shadow
	GameObject* shadowTarget = new GameObject();
	Transform* shadowTargetTransform = shadowTarget->addComponent<Transform>();
	shadowTargetTransform->position = glm::vec3(0, 0, -7);
	shadowTargetTransform->updateModel();
	delete game->shadowMapping;
	game->shadowMapping = new ShadowMapping();
	game->shadowMapping->shader = resources->getShader("shadow_mapping_depth_static");
	game->shadowMapping->target = shadowTarget;
	game->shadowMapping->lightPositionOffset = glm::vec3(5, 5, -1);
	game->shadowMapping->nearPlane = 1;
	game->shadowMapping->farPlane = 20;
	game->shadowMapping->range = 15;
	game->shadowMapping->start();
	
	// shaders
	resources->loadShader("Library", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl", "./Assets/Shaders/Fragment/mrca_4_passes.fs.glsl",  "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	
	
	
	
	
	
	
	
	
	// map
	Map* map = new Map(this, "./Assets/Models/Library/Maps/Map.fbx");
	delete map;
}

