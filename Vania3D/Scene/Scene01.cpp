
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

	Level* level = new Level("./Assets/Models/Kowloon/map.fbx");
	level->createGameObjects(this);
	delete level;


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
	
	
	
	
	
	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
	// shader configuration
	// --------------------
	game->resources->getShader("renderpass_deferred_pbr")->use();
	game->resources->getShader("renderpass_deferred_pbr")->setInt("shadowMap", 13);
	
	
	
	

	// Enable alpha channel after generate prefilter map
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Scene01::update() {
	Game* game = Game::getInstance();
	
	// lighting info
	// -------------
	glm::vec3 lightPos(-10.0f, 10.0f, -1.0f);
	
	
	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 0.0f, far_plane = 20.0f;
//	lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	game->resources->getShader("shadow_mapping_depth")->use();
	game->resources->getShader("shadow_mapping_depth")->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	Transform* transform = this->getGameObject("player")->getComponent<Transform>();
	game->resources->getShader("shadow_mapping_depth")->setMat4("model", transform->model);
	
	// bone
	std::vector<glm::mat4> pose = this->getGameObject("player")->getComponent<MeshRenderer>()->model->pose;
	for (unsigned int i = 0 ; i < pose.size() ; i++)
		this->getGameObject("player")->getComponent<MeshRenderer>()->material->shader->setMat4(("bones[" + std::to_string(i) + "]").c_str(), pose[i]);
	
	
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	MeshRenderer* meshRender = this->getGameObject("player")->getComponent<MeshRenderer>();
	meshRender->model->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	game->renderPass->begin();
	
	// 2. render scene as normal using the generated depth/shadow map
	// --------------------------------------------------------------
	game->resources->getShader("deferred_pbr_bone")->use();
	// set light uniforms
	game->resources->getShader("deferred_pbr_bone")->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	
	game->resources->getShader("renderpass_deferred_pbr")->use();
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	
	
	
	
	// show light spere, for test
	for (unsigned int i = 0; i < this->lights.size(); i ++) {
		this->lights[i]->getComponent<Transform>()->update();
		this->lights[i]->getComponent<MeshRenderer>()->update();
	}
}
