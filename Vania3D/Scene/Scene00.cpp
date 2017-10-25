
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
	// this->lightColors[1] = glm::vec3(100.0f, 100.0f, 100.0f);
	// this->lightColors[2] = glm::vec3(100.0f, 100.0f, 100.0f);
	this->lightColors[3] = glm::vec3(100.0f, 100.0f, 100.0f);
	// this->lightColors[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	this->lightColors[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	this->lightColors[2] = glm::vec3(0.0f, 0.0f, 0.0f);
	// this->lightColors[3] = glm::vec3(0.0f, 0.0f, 0.0f);

	// initialize static shader uniforms before rendering
	game->resources->getShader("deferredPBRforUEmask")->use();
		// camera
		game->resources->getShader("deferredPBRforUEmask")->setMat4("projection", this->camera->getMatrixProjection());
		// transform
		glm::mat4 model = glm::mat4();
//		model = glm::rotate(-PI, glm::vec3(1,0,0)) * model;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
		// texture
		game->resources->getShader("deferredPBRforUEmask")->setMat4("model", model);
		game->resources->getShader("deferredPBRforUEmask")->setInt("albedoMap", 0);
		game->resources->getShader("deferredPBRforUEmask")->setInt("normalMap", 1);
		game->resources->getShader("deferredPBRforUEmask")->setInt("maskMap", 2);
		// game->resources->getShader("deferredPBRforUEmask")->setInt("roughnessMap", 3);
		// game->resources->getShader("deferredPBRforUEmask")->setInt("aoMap", 4);

		std::vector<Matrix4f> Transforms = game->resources->getModel("Maw_J_Laygo")->Transforms;
		// game->resources->getModel("Maw_J_Laygo")->BoneTransform(100.0, Transforms);
		for (uint i = 0 ; i < Transforms.size() ; i++) {
				// m_pEffect->SetBoneTransform(i, Transforms[i]);
//			glm::mat4 boneTransform = {
//				Transforms[i].m[0][0],Transforms[i].m[0][1],Transforms[i].m[0][2],Transforms[i].m[0][3],
//				Transforms[i].m[1][0],Transforms[i].m[1][1],Transforms[i].m[1][2],Transforms[i].m[1][3],
//				Transforms[i].m[2][0],Transforms[i].m[2][1],Transforms[i].m[2][2],Transforms[i].m[2][3],
//				Transforms[i].m[3][0],Transforms[i].m[3][1],Transforms[i].m[3][2],Transforms[i].m[3][3]
//			};
			glm::mat4 boneTransform = {
				Transforms[i].m[0][0],Transforms[i].m[1][0],Transforms[i].m[2][0],Transforms[i].m[3][0],
				Transforms[i].m[0][1],Transforms[i].m[1][1],Transforms[i].m[2][1],Transforms[i].m[3][1],
				Transforms[i].m[0][2],Transforms[i].m[1][2],Transforms[i].m[2][2],Transforms[i].m[3][2],
				Transforms[i].m[0][3],Transforms[i].m[1][3],Transforms[i].m[2][3],Transforms[i].m[3][3]
			};
			
				game->resources->getShader("deferredPBRforUEmask")->setMat4(("bones[" + std::to_string(i) + "]").c_str(), boneTransform);
		}


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

	glm::mat4 model = glm::mat4();


// object
	// shader
	game->resources->getShader("deferredPBRforUEmask")->use();
		// camera
		game->resources->getShader("deferredPBRforUEmask")->setMat4("view", this->camera->getMatrixView());
		// transform
		// model = glm::scale(glm::vec3(0.1f));
		// model = glm::translate(glm::vec3(0.0, 0.0, 0.0));
		// game->resources->getShader("deferredPBRforUEmask")->setMat4("model", model);
		// texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("Maw_J_Laygo_albedo")->id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("Maw_J_Laygo_normal")->id);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("Maw_J_Laygo_mask")->id);
		// glActiveTexture(GL_TEXTURE3);
		// glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("WPN_AKM_roughness")->id);
		// glActiveTexture(GL_TEXTURE4);
		// glBindTexture(GL_TEXTURE_2D, game->resources->getTexture("WPN_AKM_ao")->id);
	game->resources->getModel("Maw_J_Laygo")->draw();


// renderPass
	game->resources->getShader("renderPass")->use();
	game->resources->getShader("renderPass")->setVec3("cameraPos", this->camera->getPosition());



	this->camera->updateInput(game->window->window, this->deltaTime);
}
