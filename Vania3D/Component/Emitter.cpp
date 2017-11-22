
#include "Engine.hpp"

Emitter::Emitter() {
	
}


Emitter::~Emitter() {
	
}

void Emitter::start() {

}

void Emitter::update() {
	// spawn
	if (game->time->currentTime > this->lastSpawnTime + this->spawnTime ) {
		GameObject* particle = this->activeParticle();
		if (particle) {
			this->lastSpawnTime = game->time->currentTime;;
			// Transform
			particle->getComponent<Transform>()->position = this->gameObject->getComponent<Transform>()->position;
			particle->getComponent<Transform>()->scale = glm::vec3(20 * GLOBAL_SCALE);
			particle->getComponent<UVAnimation>()->animationStartTime = game->time->currentTime;
		}
	}
	
	// update
	for (unsigned int i = 0; i < this->particles.size(); i++) {
		if (this->particles[i]->active) {
			if (game->time->currentTime > this->particles[i]->lastActiveTime + this->lifeTime) {
				this->particles[i]->active = false;
			}
			else {
				this->particles[i]->getComponent<Transform>()->position.y += 0.2 * game->time->deltaTime;
//				this->particles[i]->getComponent<Transform>()->scale -= glm::vec3(GLOBAL_SCALE / this->lifeTime) * game->time->deltaTime;
			}
		}
	}
	
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

GameObject* Emitter::activeParticle() {
	for (unsigned int i = 0; i < this->particles.size(); i++) {
		if (!this->particles[i]->active) {
			this->particles[i]->active = true;
			this->particles[i]->lastActiveTime = game->time->currentTime;
			return this->particles[i];
		}
	}
	return nullptr;
}
