
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Offset::Offset() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Offset::~Offset() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void Offset::start() {
	Transform* transform = this->gameObject->getComponent<Transform>();
	transform->position = this->parent->position + this->offsetPosition;
	transform->kinematic = this->parent->kinematic;
}

/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Offset::update() {
	if (!this->gameObject->getComponent<Transform>()->kinematic)
		this->gameObject->getComponent<Transform>()->position = this->parent->position + this->offsetPosition;
}
