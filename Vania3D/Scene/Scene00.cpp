
#include "Engine.hpp"
// generate sample kernel
std::vector<glm::vec3> genSSAOKernel(unsigned int kernelSize) {

	std::vector<glm::vec3> ssaoKernel;

	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;

	for (unsigned int i = 0; i < kernelSize; ++i) {
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / kernelSize;
		// scale samples s.t. they're more aligned to center of kernel
		scale = 0.1f + (1.0f - 0.1f) * scale * scale;
		sample *= scale;
		ssaoKernel.push_back(sample);
	}
	return ssaoKernel;
}

// generate noise texture
unsigned int genNoiseTexture(unsigned int textureSize) {

	std::vector<glm::vec3> ssaoNoise;

	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;

	for (unsigned int i = 0; i < textureSize * textureSize; i++) {
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	unsigned int noiseTexture;
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, textureSize, textureSize, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return noiseTexture;
}



// Returns a quaternion that will make your object looking towards 'direction'.
// Similar to RotationBetweenVectors, but also controls the vertical orientation.
// This assumes that at rest, the object faces +Z.
// Beware, the first parameter is a direction, not the target point !

//glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp){
//
//	if (length2(direction) < 0.0001f )
//		return glm::quat();
//
//	// Recompute desiredUp so that it's perpendicular to the direction
//	// You can skip that part if you really want to force desiredUp
//	glm::vec3 right = cross(direction, desiredUp);
//	desiredUp = cross(right, direction);
//
//	// Find the rotation between the front of the object (that we assume towards +Z,
//	// but this depends on your model) and the desired direction
//	glm::quat rot1 = rotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), direction);
//	// Because of the 1rst rotation, the up is probably completely screwed up.
//	// Find the rotation between the "up" of the rotated object, and the desired up
//	glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
//	glm::quat rot2 = rotationBetweenVectors(newUp, desiredUp);
//
//	// Apply them
//	return rot2 * rot1; // remember, in reverse order.
//}


/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Scene00::Scene00() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Scene00::~Scene00() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void Scene00::start() {
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


	//kernel
	//	std::vector<glm::vec3> ssaoKernel = genSSAOKernel(4);
	//	for (unsigned int i = 0; i < ssaoKernel.size(); ++i)
	//		game->resources->getShader("renderPass")->setVec3(("samples[" + std::to_string(i) + "]").c_str(), ssaoKernel[i]);
	// noiseTexture
	//	unsigned int noiseTexture = genNoiseTexture(4);
	//	glActiveTexture(GL_TEXTURE9);
	//	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	//	game->resources->getShader("renderPass")->setInt("texNoise", 9);
	
	
	// Enable alpha channel after generate prefilter map
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Scene00::update() {
	// show light spere, for test
	for (unsigned int i = 0; i < this->lights.size(); i ++) {
		this->lights[i]->getComponent<Transform>()->update();
		this->lights[i]->getComponent<MeshRenderer>()->update();
	}
}
