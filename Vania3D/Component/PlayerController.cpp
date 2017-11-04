
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
PlayerController::PlayerController() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
PlayerController::~PlayerController() {

}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void PlayerController::update() {
	GameObject* gameObject = this->getGameObject();
	Transform* playerTransform = gameObject->getComponent<Transform>();

	glm::vec3 direction = playerTransform->front(); // if no input deflaut the last direction
	glm::vec3 axisLS = game->input->getAxisLS();

	glm::vec3 cameraFrontFromWorldUp = glm::normalize(glm::cross(this->camera->getComponent<Camera>()->cameraRight, this->camera->getComponent<Camera>()->worldUp));
	glm::quat worldToCamera = glm::rotation(glm::vec3(0,0,1), cameraFrontFromWorldUp);


	if (game->time->currentTime - this->lastAttack > 3.0) {
		if (abs(axisLS.x) > 0.6 || abs(axisLS.z) > 0.6) {
			direction = worldToCamera * game->input->getNormalLS();
			axisLS = worldToCamera * axisLS;

			playerTransform->position.x += 20 * axisLS.x * game->time->deltaTime;
			playerTransform->position.z += 20 * axisLS.z * game->time->deltaTime;

			this->animation = 3;
		}
		else if (abs(axisLS.x) > 0.1 || abs(axisLS.z) > 0.1){
			direction = worldToCamera * game->input->getNormalLS();
			axisLS = worldToCamera * axisLS;

			playerTransform->position.x += 10 * axisLS.x * game->time->deltaTime;
			playerTransform->position.z += 10 * axisLS.z * game->time->deltaTime;

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

	playerTransform->rotate(direction, 2 * PI * game->time->deltaTime);
	
	gameObject->getComponent<MeshRenderer>()->model->updatePose(this->animation, game->time->currentTime);
}
