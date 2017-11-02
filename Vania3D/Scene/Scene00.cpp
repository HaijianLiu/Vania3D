
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
	

	/* GameObject */
	this->transform = new Transform();
	this->transform->modelScale = glm::vec3(0.05);

	// camera
	this->camera = new Camera();
	this->cameraController = new CameraController();
	this->cameraController->camera = this->camera;
	this->cameraController->transform = this->transform;
	this->camera->target = this->transform;
	this->camera->offsetFromTarget = this->camera->position - (this->camera->target->position + this->camera->offset);

	/* light */
	this->lightPositions[0] = glm::vec3(-10.0f,  10.0f, 20.0f);
	this->lightPositions[1] = glm::vec3( 10.0f,  10.0f, 20.0f);
	this->lightPositions[2] = glm::vec3(-10.0f, -10.0f, 20.0f);
	this->lightPositions[3] = glm::vec3( 10.0f, -10.0f, 20.0f);
	this->lightColors[0] = glm::vec3(100.0f, 100.0f, 100.0f);
	this->lightColors[1] = glm::vec3(100.0f, 100.0f, 100.0f);
	this->lightColors[2] = glm::vec3(100.0f, 100.0f, 100.0f);
	this->lightColors[3] = glm::vec3(100.0f, 100.0f, 100.0f);
	// this->lightColors[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	// this->lightColors[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	// this->lightColors[2] = glm::vec3(0.0f, 0.0f, 0.0f);
	// this->lightColors[3] = glm::vec3(0.0f, 0.0f, 0.0f);
	
	// camera
	
//	light
	game->resources->getShader("simple")->use();
		game->resources->getShader("simple")->setMat4("projection", this->camera->projection);



	// initialize static shader uniforms before rendering
	game->resources->getShader("deferredPBRforUEmask")->use();
		// camera
		game->resources->getShader("deferredPBRforUEmask")->setMat4("projection", this->camera->projection);
		// texture
		game->resources->getShader("deferredPBRforUEmask")->setInt("albedoMap", 0);
		game->resources->getShader("deferredPBRforUEmask")->setInt("normalMap", 1);
		game->resources->getShader("deferredPBRforUEmask")->setInt("maskMap", 2);
		// game->resources->getShader("deferredPBRforUEmask")->setInt("roughnessMap", 3);
		// game->resources->getShader("deferredPBRforUEmask")->setInt("aoMap", 4);

	// IBL
	game->renderPass->setActiveLightProbe(game->resources->getLightProbe("hdr"));

	game->resources->getShader("renderPass")->use();
	//kernel
	std::vector<glm::vec3> ssaoKernel = genSSAOKernel(4);
	for (unsigned int i = 0; i < ssaoKernel.size(); ++i)
		game->resources->getShader("renderPass")->setVec3(("samples[" + std::to_string(i) + "]").c_str(), ssaoKernel[i]);
	// noiseTexture
	unsigned int noiseTexture = genNoiseTexture(4);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	game->resources->getShader("renderPass")->setInt("texNoise", 9);
	// camera
	game->resources->getShader("renderPass")->setMat4("projection", this->camera->projection);
	// lights
	for (unsigned int i = 0; i < sizeof(this->lightPositions) / sizeof(this->lightPositions[0]); ++i) {
		game->resources->getShader("renderPass")->setVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
		game->resources->getShader("renderPass")->setVec3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
	}

	// Enable alpha channel after generate prefilter map
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Scene00::update() {
	Game* game = Game::getInstance();

//	camera
	this->transform->update();

	this->camera->update();

	// shader
	game->resources->getShader("deferredPBRforUEmask")->use();
		// camera
		game->resources->getShader("deferredPBRforUEmask")->setMat4("view", this->camera->view);

		// input
		// Compute the desired orientation
	// begin from non rotate

//	controll
	glm::vec3 direction = this->transform->front(); // if no input deflaut the last direction
	glm::vec3 axisLS = game->input->getAxisLS();

	glm::vec3 cameraFrontFromWorldUp = glm::normalize(glm::cross(this->camera->cameraRight, this->camera->worldUp));
	glm::quat worldToCamera = glm::rotation(glm::vec3(0,0,1), cameraFrontFromWorldUp);


	if (game->time->currentTime - this->lastAttack > 3.0) {
		if (abs(axisLS.x) > 0.6 || abs(axisLS.z) > 0.6) {
			direction = worldToCamera * game->input->getNormalLS();
			axisLS = worldToCamera * axisLS;

			this->transform->position.x += 20 * axisLS.x * game->time->deltaTime;
			this->transform->position.z += 20 * axisLS.z * game->time->deltaTime;

			this->animation = 3;
		}
		else if (abs(axisLS.x) > 0.1 || abs(axisLS.z) > 0.1){
			direction = worldToCamera * game->input->getNormalLS();
			axisLS = worldToCamera * axisLS;

			this->transform->position.x += 10 * axisLS.x * game->time->deltaTime;
			this->transform->position.z += 10 * axisLS.z * game->time->deltaTime;

			this->animation = 2;
		}
		else {
			this->animation = 0;
		}
	}

	if (game->input->getJoystickTrigger(JOY_L1)) {
		this->animation = 4;
		this->lastAttack = game->time->currentTime;
	}

	this->transform->rotate(direction, 2 * PI * game->time->deltaTime);


//	camera controller
	glm::vec3 axisRS = game->input->getAxisRS();
	this->camera->rotate(-2 * axisRS.x * game->time->deltaTime, axisRS.z * game->time->deltaTime);
	if (game->input->getJoystickPress(JOY_L2))
		this->camera->zoom(game->time->deltaTime * 6);
	if (game->input->getJoystickPress(JOY_R2))
		this->camera->zoom(-game->time->deltaTime * 6);



	/* render */
		game->resources->getShader("deferredPBRforUEmask")->setMat4("model", this->transform->model);

		// texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("vampire_albedo")->id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("vampire_normal")->id);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("vampire_mask")->id);
		// glActiveTexture(GL_TEXTURE3);
		// glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("WPN_AKM_roughness")->id);
		// glActiveTexture(GL_TEXTURE4);
		// glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("WPN_AKM_ao")->id);


	game->resources->getModel("vampire")->updatePose(this->animation, game->time->currentTime);


	std::vector<glm::mat4> Transforms = game->resources->getModel("vampire")->pose;
		// game->resources->getModel("Maw_J_Laygo")->BoneTransform(100.0, Transforms);
		for (uint i = 0 ; i < Transforms.size() ; i++) {
			glm::mat4 boneTransform = Transforms[i];

				game->resources->getShader("deferredPBRforUEmask")->setMat4(("bones[" + std::to_string(i) + "]").c_str(), boneTransform);
		}

	game->resources->getModel("vampire")->draw();

	// light
	game->resources->getShader("simple")->use();
		game->resources->getShader("simple")->setMat4("view", this->camera->view);
		glm::mat4 lightScale = glm::scale(glm::vec3(0.1));
		glm::mat4 lightTranslate = glm::translate(this->lightPositions[0]);
		glm::mat4 modelLight = lightTranslate * lightScale;
		game->resources->getShader("simple")->setMat4("model", modelLight);
	game->resources->sphere->draw();

// renderPass
	game->resources->getShader("renderPass")->use();
	game->resources->getShader("renderPass")->setVec3("cameraPos", this->camera->position);


}
