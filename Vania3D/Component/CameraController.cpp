
#include "Engine.hpp"


CameraController::CameraController() {
	this->transform = new Transform();
}

CameraController::~CameraController() {
	delete this->transform;
}
