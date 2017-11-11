
#include "Engine.hpp"

Camera::Camera() {

}


Camera::~Camera() {

}


void Camera::update() {
	Transform* transform = this->gameObject->getComponent<Transform>();
	transform->position = this->target->position + this->offsetFromTarget;

	// calculate the new front vector
	this->front = glm::normalize(-this->offsetFromTarget);
	this->right = glm::normalize(glm::cross(this->front, this->game->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));

	// view matrix (camera position (in world space), target position (in world space), camera up direction)
	this->view = glm::lookAt(transform->position, transform->position + this->front, this->up);
	
	// projection matrix (field of view, ratio, display range : start unit <-> end units)
	this->projection = glm::perspective(glm::radians(this->field),this->ratio,this->rangeStart,this->rangeEnd);
}


void Camera::rotate(float radiansHorizonal, float radiansVertical) {
	// update offset from target vec3

	// horizonal
	glm::quat rotationHorizonal = glm::angleAxis(radiansHorizonal, this->game->worldUp);
	this->offsetFromTarget = rotationHorizonal * this->offsetFromTarget;

	// vertical
	glm::quat rotationVertical = glm::angleAxis(radiansVertical, this->right);
	glm::vec3 tempOffset = rotationVertical * this->offsetFromTarget;
	float radiansToWorldUp = glm::dot(glm::normalize(tempOffset), this->game->worldUp);
	if (radiansToWorldUp < 0.8 && radiansToWorldUp > 0) {
		this->offsetFromTarget = tempOffset;
	}
}


void Camera::zoom(float distance) {
	glm::vec3 zoomDistance = this->front * distance;
	glm::vec3 tempOffset = this->offsetFromTarget + zoomDistance;
	float length2 = glm::length2(tempOffset);
	if (length2 > 100 && length2 <= 500) {
		this->offsetFromTarget = tempOffset;
	}
}
