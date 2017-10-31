
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Transform::Transform() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Transform::~Transform() {

}


/*------------------------------------------------------------------------------
< update > in GameObject draw()
------------------------------------------------------------------------------*/
void Transform::update() {
	this->model = glm::translate(this->position) * glm::scale(this->scale * this->modelScale) * glm::mat4_cast(this->rotation);
}


/*------------------------------------------------------------------------------
< get methods >
------------------------------------------------------------------------------*/
glm::vec3 Transform::front() {
	return glm::normalize(this->rotation * this->modelFront);
}


/*------------------------------------------------------------------------------
< rotate to desired vec3 by radians >
like slerp, but forbids rotation greater than max angle (in radians)
------------------------------------------------------------------------------*/
void Transform::rotate(glm::vec3 direction, float radians){
	// no rotation allowed & prevent dividing by 0 later
	// smaller threshold will makes a smoother rotation
	if(radians < 0.0001) return;

	glm::vec3 start = glm::normalize(this->modelFront);
	glm::vec3 end = glm::normalize(direction);
	float cosTheta = glm::dot(start, end); // vec dot value is different from quat dot value

	// calculate direction quaternion
	glm::vec3 rotationAxis;
	glm::quat directionQuaternion;
	if (cosTheta < -1 + 0.0001){
		// special case when vectors in opposite directions :
		// there is no "ideal" rotation axis
		// so guess one; any will do as long as it's perpendicular to start
		// this implementation favors a rotation around the Up axis,
		rotationAxis = glm::cross(glm::vec3(0.0, 0.0, 1.0), start);
		if (glm::length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
			rotationAxis = glm::cross(glm::vec3(1.0, 0.0, 0.0), start);
		rotationAxis = glm::normalize(rotationAxis);
		directionQuaternion = glm::angleAxis((float)glm::radians(180.0), rotationAxis);
	}
	else {
		// Implementation from Stan Melax's Game Programming Gems 1 article
		rotationAxis = cross(start, end);
		float s = glm::sqrt( (1+cosTheta)*2 );
		float invs = 1 / s;
		directionQuaternion = glm::quat(s * 0.5, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);
	}

	float cosQuat = glm::dot(this->rotation, directionQuaternion); // vec dot value is different from quat dot value
	// when they are already equal no rotation allowed
	// bigger threshold will makes a more sensitivity rotation
	if(cosQuat > 0.9999) return;
	// avoid taking the long path around the sphere
	if (cosQuat < 0.0){
		this->rotation *= -1.0;
		cosQuat *= -1.0;
	}

	float deltaTheta = glm::acos(cosQuat);

	// If there is only a 2° difference, and we are allowed 5°,
	if (radians > deltaTheta){
		this->rotation = directionQuaternion;
	}
	else {
		// This is just like slerp(), but with a custom t
		float factor = radians / deltaTheta;
		this->rotation = glm::slerp(this->rotation, directionQuaternion, factor);
	}
}
