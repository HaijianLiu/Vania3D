
#include "Engine.hpp"

Camera::Camera() {
	// Camera Position
	this->cameraFront = glm::vec3(0.0,0.0,-1.0);
	this->cameraUp    = glm::vec3(0.0,1.0,0.0);
	this->cameraRight = glm::vec3(1.0,0.0,0.0);
	this->worldFront  = glm::vec3(0.0,0.0,1.0);
	this->worldUp     = glm::vec3(0.0,1.0,0.0);
	this->worldRight  = glm::vec3(1.0,0.0,0.0);

	// Moment options
	this->moveSpeed = 8.0;
	this->rotateSensitivity = 40.0;
	// Camera Attributes
	this->field      = 45.0;
	this->ratio      = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	this->rangeStart = 0.1;
	this->rangeEnd   = 100.0;

	// projection matrix (field of view, ratio, display range : start unit <-> end units)
	this->projection = glm::perspective(glm::radians(this->field),this->ratio,this->rangeStart,this->rangeEnd);

	this->offset = glm::vec3(0, 5, 0);
}


Camera::~Camera() {

}


void Camera::update() {
	Transform* transform = this->getGameObject()->getComponent<Transform>();
	transform->position = this->target->position + this->offset + this->offsetFromTarget;

	// calculate the new front vector
	this->cameraFront = glm::normalize(-this->offsetFromTarget);
	this->cameraRight = glm::normalize(glm::cross(this->cameraFront, this->worldUp));
	this->cameraUp = glm::normalize(glm::cross(this->cameraRight, this->cameraFront));

	// view matrix (camera position (in world space), target position (in world space), camera up direction)
	this->view = glm::lookAt(transform->position, transform->position + this->cameraFront, this->cameraUp);
}


void Camera::rotate(float radiansHorizonal, float radiansVertical) {
	// update offset from target vec3

	// horizonal
	glm::quat rotationHorizonal = glm::angleAxis(radiansHorizonal, this->worldUp);
	this->offsetFromTarget = rotationHorizonal * this->offsetFromTarget;

	// vertical
	glm::quat rotationVertical = glm::angleAxis(radiansVertical, this->cameraRight);
	glm::vec3 tempOffset = rotationVertical * this->offsetFromTarget;
	float radiansToWorldUp = glm::dot(glm::normalize(tempOffset), this->worldUp);
	if (radiansToWorldUp < 0.8 && radiansToWorldUp > 0) {
		this->offsetFromTarget = tempOffset;
	}
}


void Camera::zoom(float distance) {
	glm::vec3 zoomDistance = this->cameraFront * distance;
	glm::vec3 tempOffset = this->offsetFromTarget + zoomDistance;
	float length2 = glm::length2(tempOffset);
	if (length2 > 100 && length2 <= 500) {
		this->offsetFromTarget = tempOffset;
	}
}
