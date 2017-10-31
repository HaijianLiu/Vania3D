
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Time::Time() {

}


Time* Time::getInstance() {
	static Time* time = new Time();
	return time;
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Time::~Time() {

}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void Time::update() {
	this->currentTime = glfwGetTime();
	this->deltaTime = this->currentTime - this->lastTime;
	this->deltaTime > 0.04f ? 0.032f : this->deltaTime = 0.032f; // 30 FPS lastest
	this->lastTime = this->currentTime;
}
