
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< callback >
------------------------------------------------------------------------------*/
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Input::getInstance()->keycallback(window,key,scancode,action,mods);
}

void joystickCallback(int joy, int event) {
	Input::getInstance()->joystickcallback(joy,event);
}

Input* Input::getInstance() {
	static Input instance;
	return &instance;
}

void Input::keycallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	this->keys[key] = action;
	if(action == GLFW_REPEAT ) {
		std::cout << "GLFW_REPEAT" << key << std::endl;
	} else if (action == GLFW_PRESS) {
		std::cout << "GLFW_PRESS" << key << std::endl;
	} else if (action == GLFW_RELEASE) {
		std::cout << "GLFW_RELEASE" << key << std::endl;
	}
}

void Input::joystickcallback(int joy, int event) {
	this->joyEvent = event;
	this->joyConnect = joy;
	if (event == GLFW_CONNECTED) {
		std::cout << "GLFW_CONNECTED : " << joy << std::endl;
	}
	if (event == GLFW_DISCONNECTED) {
		std::cout << "GLFW_DISCONNECTED : " << joy << std::endl;
	}
}


/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Input::Input() {
	Game* game = Game::getInstance();
	glfwSetKeyCallback(game->window->window, keyCallback);
	glfwSetJoystickCallback(joystickCallback);
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Input::~Input() {

}


/*------------------------------------------------------------------------------
< get button action >
------------------------------------------------------------------------------*/
bool Input::getButtonPress(int button) {
	if (this->keys[button] == GLFW_PRESS || this->keys[button] == GLFW_REPEAT) return true;
	else return false;
}

bool Input::getButtonTrigger(int button) {
	if (this->keys[button] == GLFW_PRESS) {
		this->keys[button] = GLFW_REPEAT; return true;
	}
	else return false;
}

bool Input::getJoystickPress(int button) {
	this->updateJoystick(button);
	if (this->joyButtons[button] == GLFW_PRESS || this->joyButtons[button] == GLFW_REPEAT) return true;
	else return false;
	return false;
}

bool Input::getJoystickTrigger(int button) {
	this->updateJoystick(button);
	if (this->joyButtons[button] == GLFW_PRESS) return true;
	else return false;
}

void Input::updateJoystick(int button) {
	if (this->joyEvent == GLFW_CONNECTED) {
		const unsigned char* buttons = glfwGetJoystickButtons(this->joyConnect, &this->joyButtonCount);
		if (buttons[button] == GLFW_PRESS && (this->joyButtons[button] == GLFW_PRESS || this->joyButtons[button] == GLFW_REPEAT)) {
			this->joyButtons[button] = GLFW_REPEAT;
			// std::cout << this->joyButtons[button] << std::endl;
		}
		else {
			this->joyButtons[button] = buttons[button];
			// std::cout << this->joyButtons[button] << std::endl;
		}
	}
}



// const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1,&count);
// if (buttons[2] == GLFW_PRESS) {
// 	 std::cout << "2 is press" << std::endl;
// }
// if (buttons[2] == GLFW_RELEASE) {
// 	std::cout << "2 is release" << std::endl;
// }
//    std::cout << buttons[0] << std::endl;
