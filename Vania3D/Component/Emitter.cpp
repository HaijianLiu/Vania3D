
#include "Engine.hpp"

Emitter::Emitter() {
	
}


Emitter::~Emitter() {
	
}

void Emitter::start() {
	
}

void Emitter::update() {
	
}

void Emitter::createParticles(const char* name, Scene* scene) {
	std::string particleName = std::string(name);
	for (unsigned int i = 0; i < this->maxParticles; i++) {
		GameObject* particle = new GameObject();
		particle->staticObject = false;
		particle->active = false;
		Transform* particleTransform = particle->addComponent<Transform>();
		particleTransform->position = glm::vec3(0, 1, 0);
		particleTransform->scale = glm::vec3(20 * GLOBAL_SCALE);
		MeshRenderer* particleMeshRenderer = particle->addComponent<MeshRenderer>();
		particleMeshRenderer->model = game->resources->getModel("Fire1");
		particleMeshRenderer->materials.push_back(game->resources->getMaterial("Fire1"));
		particleMeshRenderer->renderLayer = RENDER_LAYER_FX;
		particle->addComponent<Billboard>();
		UVAnimation* particleUVAnimation = new UVAnimation(4, 4, 0.1);
		particle->addComponent(particleUVAnimation);
		scene->addGameObject((particleName + std::to_string(i)).c_str(), particle);
		this->particles.push_back(particle);
	}
}
