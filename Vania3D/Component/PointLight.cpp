
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
PointLight::PointLight() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
PointLight::~PointLight() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void PointLight::start() {
	this->radius = 10 * glm::sqrt(this->intensity);
}
