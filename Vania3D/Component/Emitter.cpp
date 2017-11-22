
#include "Engine.hpp"

Emitter::Emitter() {
	
}


Emitter::~Emitter() {
	
}

void Emitter::start() {

}

void Emitter::update() {
	if (game->time->currentTime > this->lastSpawnTime + this->spawnTime) {
		this->activeParticle();
		this->lastSpawnTime = game->time->currentTime;
	}
}

void Emitter::createParticles(const char* name, Scene* scene) {
	std::string particleName = std::string(name);
	for (unsigned int i = 0; i < this->maxParticles; i++) {
		GameObject* particle = new GameObject();
		particle->staticObject = false;
		particle->active = false;
		Transform* particleTransform = particle->addComponent<Transform>();
		MeshRenderer* particleMeshRenderer = particle->addComponent<MeshRenderer>();
		particleMeshRenderer->model = game->resources->getModel("Fire1");
		particleMeshRenderer->materials.push_back(game->resources->getMaterial("Fire1"));
		particleMeshRenderer->renderLayer = RENDER_LAYER_FX;
		particle->addComponent<Billboard>();
		UVAnimation* particleUVAnimation = new UVAnimation(4, 4, 0.1);
		particle->addComponent(particleUVAnimation);
		Particle* particleComponent = particle->addComponent<Particle>();
		scene->addGameObject((particleName + std::to_string(i)).c_str(), particle);
		this->particles.push_back(particleComponent);
	}
}

void Emitter::activeParticle() {
	for (unsigned int i = 0; i < this->particles.size(); i++) {
		if (!this->particles[i]->gameObject->active) {
			this->particles[i]->gameObject->active = true;
			this->particles[i]->gameObject->lastActiveTime = game->time->currentTime;
			this->particles[i]->gameObject->transform->position = this->gameObject->transform->position;
			this->particles[i]->initScale = glm::vec3(10 * GLOBAL_SCALE);
			this->particles[i]->initVelocity = glm::vec3(glm::linearRand(-0.05, 0.05), glm::linearRand(-0.1, 0.1), glm::linearRand(-0.05, 0.05));
			this->particles[i]->lifeTime = glm::linearRand(1.0, 1.5);
			return;
		}
	}
}
