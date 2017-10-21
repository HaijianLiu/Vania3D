
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
	this->model = glm::mat4();
	this->model = glm::rotate(this->rotation.y, glm::vec3(0,1,0));
	this->model = glm::scale(this->model, this->rotation);
	this->model = glm::translate(this->model, this->position);
}
