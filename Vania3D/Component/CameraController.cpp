
#include "Engine.hpp"


CameraController::CameraController() {

}


CameraController::~CameraController() {

}


void CameraController::update() {
	Camera* camera = this->camera->getComponent<Camera>();
	glm::vec3 axisRS = game->input->getAxisRS();
	camera->rotate(-2 * axisRS.x * game->time->deltaTime, axisRS.z * game->time->deltaTime);
	if (game->input->getJoystickPress(JOY_L2))
		camera->zoom(game->time->deltaTime * 6);
	if (game->input->getJoystickPress(JOY_R2))
		camera->zoom(-game->time->deltaTime * 6);
}
