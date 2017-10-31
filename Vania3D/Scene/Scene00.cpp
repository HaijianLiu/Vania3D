
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


// Returns a quaternion such that q*start = dest
glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest){
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){
		// special case when vectors in opposite directions :
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		// This implementation favors a rotation around the Up axis,
		// since it's often what you want to do.
		rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
			rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return angleAxis(glm::radians(180.0f), rotationAxis);
	}

	// Implementation from Stan Melax's Game Programming Gems 1 article
	rotationAxis = cross(start, dest);

	float s = sqrt( (1+cosTheta)*2 );
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);

}



// Returns a quaternion that will make your object looking towards 'direction'.
// Similar to RotationBetweenVectors, but also controls the vertical orientation.
// This assumes that at rest, the object faces +Z.
// Beware, the first parameter is a direction, not the target point !
glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp){

	if (length2(direction) < 0.0001f )
		return glm::quat();

	// Recompute desiredUp so that it's perpendicular to the direction
	// You can skip that part if you really want to force desiredUp
	glm::vec3 right = cross(direction, desiredUp);
	desiredUp = cross(right, direction);

	// Find the rotation between the front of the object (that we assume towards +Z,
	// but this depends on your model) and the desired direction
	glm::quat rot1 = RotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), direction);
	// Because of the 1rst rotation, the up is probably completely screwed up.
	// Find the rotation between the "up" of the rotated object, and the desired up
	glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
	glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);

	// Apply them
	return rot2 * rot1; // remember, in reverse order.
}



// Like SLERP, but forbids rotation greater than maxAngle (in radians)
// In conjunction to LookAt, can make your characters
glm::quat RotateTowards(glm::quat q1, glm::quat q2, float maxAngle){

	if( maxAngle < 0.001f ){
		// No rotation allowed. Prevent dividing by 0 later.
		return q1;
	}

	float cosTheta = dot(q1, q2);

	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if(cosTheta > 0.9f){
		return q2;
	}

	// Avoid taking the long path around the sphere
	if (cosTheta < 0){
		q1 = q1*-1.0f;
		cosTheta *= -1.0f;
	}

	float angle = acos(cosTheta);

	// If there is only a 2° difference, and we are allowed 5°,
	// then we arrived.
	if (angle < maxAngle){
		return q2;
	}

	// This is just like slerp(), but with a custom t
	float t = maxAngle / angle;
	angle = maxAngle;

	glm::quat res = (sin((1.0f - t) * angle) * q1 + sin(t * angle) * q2) / sin(angle);
	res = normalize(res);
	return res;

}


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
	this->camera = new Camera();

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

	// initialize static shader uniforms before rendering
	game->resources->getShader("deferredPBRforUEmask")->use();
		// camera
		game->resources->getShader("deferredPBRforUEmask")->setMat4("projection", this->camera->getMatrixProjection());
		// texture
		game->resources->getShader("deferredPBRforUEmask")->setInt("albedoMap", 0);
		game->resources->getShader("deferredPBRforUEmask")->setInt("normalMap", 1);
		game->resources->getShader("deferredPBRforUEmask")->setInt("maskMap", 2);
		// game->resources->getShader("deferredPBRforUEmask")->setInt("roughnessMap", 3);
		// game->resources->getShader("deferredPBRforUEmask")->setInt("aoMap", 4);

	// std::vector<Matrix4> Transforms = game->resources->getModel("vampire")->pose;
	// 	// game->resources->getModel("Maw_J_Laygo")->BoneTransform(100.0, Transforms);
	// 	for (uint i = 0 ; i < Transforms.size() ; i++) {
	// 		glm::mat4 boneTransform = Transforms[i].getGLM();
	//
	// 			game->resources->getShader("deferredPBRforUEmask")->setMat4(("bones[" + std::to_string(i) + "]").c_str(), boneTransform);
	// 	}


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
	game->resources->getShader("renderPass")->setMat4("projection", this->camera->getMatrixProjection());
	// lights
	for (unsigned int i = 0; i < sizeof(this->lightPositions) / sizeof(this->lightPositions[0]); ++i) {
		game->resources->getShader("renderPass")->setVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
		game->resources->getShader("renderPass")->setVec3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
	}
	// Enable alpha channel
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Scene00::update() {
	Game* game = Game::getInstance();



	// per-frame time logic
	float currentFrame = glfwGetTime();
	this->deltaTime = currentFrame - this->lastFrame;
	this->lastFrame = currentFrame;
	deltaTime > 1.0 ? 1.0 : deltaTime;

// object
	// shader
	game->resources->getShader("deferredPBRforUEmask")->use();
		// camera
		game->resources->getShader("deferredPBRforUEmask")->setMat4("view", this->camera->getMatrixView());

		// input
		// Compute the desired orientation
	glm::vec3 desiredDir = this->front;
//	float currentTimeDead;

	if (currentFrame - this->lastAttack > 3.0) {
		const float* input = game->input->axis();
		float axis[6];
		if (input != nullptr) {
			for (int i = 0; i < 6; i++) {
				int a = input[i] * 10;
				axis[i] = a / 10.0;
			}
			std::cout << axis[0] << " : " << axis[1] << std::endl;
			// std::cout << axis[0] << std::endl;
			if (abs(axis[0]) > 0.5 || abs(axis[1]) > 0.5) {
				this->position.x += 20 * axis[0] * deltaTime;
				this->position.z += 20 * axis[1] * deltaTime;
					desiredDir.x = input[0];
					desiredDir.z = input[1];
//					desiredDir = normalize(desiredDir);
				
				this->animation = 3;
			}
			else if (abs(axis[0]) > 0.0 || abs(axis[1]) > 0.0){
				this->position.x += 10 * axis[0] * deltaTime;
				this->position.z += 10 * axis[1] * deltaTime;
				
					desiredDir.x = input[0];
					desiredDir.z = input[1];
//					desiredDir = normalize(desiredDir);
				
				this->animation = 2;
			}
			else {
				this->animation = 0;
			}
		}

	}




	if (game->input->getJoystickTrigger(JOY_L1)) {
		this->lastAttack = currentFrame;
		this->animation = 4;
	}

	// Compute the desired orientation
//	glm::quat targetOrientation = normalize(LookAt(desiredDir, desiredDir));

	// And interpolate
//	glm::quat gOrientation2 = RotateTowards(gOrientation2, targetOrientation, 1.0 * deltaTime);


		// transform
	glm::quat rotationQuat = RotationBetweenVectors(glm::vec3(0,0,1), desiredDir);
	glm::quat finalRotationQ = RotateTowards(this->lastRotation, rotationQuat, 2*PI * deltaTime);
	this->lastRotation = finalRotationQ;
	glm::mat4 rotation = glm::mat4_cast(finalRotationQ);

	this->lastRotation = finalRotationQ;
		this->front = finalRotationQ * glm::vec3(0,0,1);
	

//	if ( abs(finalRotationQ.w - rotationQuat.w) > 0.01) {
//		this->front = finalRotationQ * this->front;
//	}



		glm::mat4 scaling = glm::scale(glm::vec3(0.05f));
		glm::mat4 position = glm::translate(this->position);
	glm::mat4 model = position * scaling * rotation;
		game->resources->getShader("deferredPBRforUEmask")->setMat4("model", model);

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


	game->resources->getModel("vampire")->updatePose(this->animation, currentFrame);


	std::vector<glm::mat4> Transforms = game->resources->getModel("vampire")->pose;
		// game->resources->getModel("Maw_J_Laygo")->BoneTransform(100.0, Transforms);
		for (uint i = 0 ; i < Transforms.size() ; i++) {
			glm::mat4 boneTransform = Transforms[i];

				game->resources->getShader("deferredPBRforUEmask")->setMat4(("bones[" + std::to_string(i) + "]").c_str(), boneTransform);
		}

	game->resources->getModel("vampire")->draw();


// renderPass
	game->resources->getShader("renderPass")->use();
	game->resources->getShader("renderPass")->setVec3("cameraPos", this->camera->getPosition());



	this->camera->updateInput(game->window->window, this->deltaTime);
}
