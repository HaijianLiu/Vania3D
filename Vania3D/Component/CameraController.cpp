
#include "Engine.hpp"


CameraController::CameraController() {

}


CameraController::~CameraController() {

}


void CameraController::update() {
	glm::vec3 axisRS = game->input->getAxisRS();
	this->camera->rotate(-2 * axisRS.x * game->time->deltaTime, axisRS.z * game->time->deltaTime);
	if (game->input->getJoystickPress(JOY_L2))
		this->camera->zoom(game->time->deltaTime * 6);
	if (game->input->getJoystickPress(JOY_R2))
		this->camera->zoom(-game->time->deltaTime * 6);
}
