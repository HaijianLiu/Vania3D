
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
	static Input* instance = new Input();
	return instance;
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

const float* Input::axis() {
	if (this->joyEvent == GLFW_CONNECTED) {
		int axesCount;
		return glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		// std::cout << axesCount << '\n';
		// std::cout << "Left  X : " << axes[0] << std::endl;
		// std::cout << "Left  Y : " << axes[1] << std::endl;
		// std::cout << "Right X : " << axes[2] << std::endl;
		// std::cout << "Right X : " << axes[3] << std::endl;
		// std::cout << "L2      : " << axes[4] << std::endl;
		// std::cout << "R2      : " << axes[5] << std::endl;
	}
	else {
		return nullptr;
	}
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
