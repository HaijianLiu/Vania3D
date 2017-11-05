
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
	return glm::normalize(this->rotation * this->game->worldFront);
}


/*------------------------------------------------------------------------------
< rotate to desired vec3 by radians >
like slerp, but forbids rotation greater than max angle (in radians)
------------------------------------------------------------------------------*/
void Transform::rotate(glm::vec3 direction, float radians) {
	// no rotation allowed & prevent dividing by 0 later
	// smaller threshold will makes a smoother rotation
	if (radians < 0.001) return;

	// get direction quaternion
	glm::quat directionQuaternion = glm::rotation(this->game->worldFront, direction);

	/* debug log */
	// std::cout << "rotation = "
	// << "w: " << directionQuaternion.w
	// << " x: " << directionQuaternion.x
	// << " y: " << directionQuaternion.y
	// << " z: " << directionQuaternion.z << std::endl;

	// update rotation
	float cosTheta = glm::dot(this->rotation, directionQuaternion); // vec dot value is different from quat dot value

	// bigger threshold will makes a more sensitivity rotation
	// when they are already equal, no rotation allowed
	if (cosTheta > 0.9999) return;
	// when they are already opposite, just turn around
	if (cosTheta < -0.9999) {
		this->rotation *= -1.0;
		return;
	}

	// avoid taking the long path around the sphere
	if (cosTheta < 0.0){
		this->rotation *= -1.0;
		cosTheta *= -1.0;
	}

	float deltaTheta = glm::acos(cosTheta);

	// If there is only a 2° difference, and we are allowed 5°,
	if (radians > deltaTheta) {
		this->rotation = directionQuaternion;
	}
	else {
		// This is just like slerp(), but with a custom t
		float factor = radians / deltaTheta;

		/* debug log */
		// std::cout << "radians: " << radians << '\n';
		// std::cout << "deltaTheta: " << deltaTheta << '\n';
		// std::cout << "factor: " << factor << '\n';

		this->rotation = glm::slerp(this->rotation, directionQuaternion, factor);
	}

	/* debug log */
	// std::cout << "rotation = "
	// << "w: " << this->rotation.w
	// << " x: " << this->rotation.x
	// << " y: " << this->rotation.y
	// << " z: " << this->rotation.z << std::endl;
}
