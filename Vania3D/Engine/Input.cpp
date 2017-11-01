
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
	if (this->joyButtons[button] == GLFW_PRESS || this->joyButtons[button] == GLFW_REPEAT) return true;
	else return false;
}

bool Input::getJoystickTrigger(int button) {
	if (this->joyButtons[button] == GLFW_PRESS) return true;
	else return false;
}


void Input::updateJoystick() {
	if (this->joyEvent == GLFW_CONNECTED) {
		
		/* buttons */
		const unsigned char* buttons = glfwGetJoystickButtons(this->joyConnect, &this->joyButtonCount);
		for (int i = 0; i < this->joyButtonCount; i++) {
			if (buttons[i] == GLFW_PRESS && (this->joyButtons[i] == GLFW_PRESS || this->joyButtons[i] == GLFW_REPEAT)) {
				this->joyButtons[i] = GLFW_REPEAT;
			}
			else {
				this->joyButtons[i] = buttons[i];
			}
		}

		/* axis */
		// get input data
		int axesCount;
		const float* joyAxis = glfwGetJoystickAxes(this->joyConnect, &axesCount);

		// scaled radial dead zone
		this->axisLS = glm::vec3(joyAxis[0], 0.0, joyAxis[1]);
		float magnitude = glm::length(this->axisLS);
		if(magnitude < this->deadzone) {
			this->normalLS = glm::vec3(0.0);
			this->axisLS = glm::vec3(0.0);
		}
		else {
			this->normalLS = glm::normalize(this->axisLS);
			this->axisLS = this->normalLS * ((magnitude - this->deadzone) / (1 - this->deadzone));
		}

		// std::cout << axesCount << '\n';
		// std::cout << "Left  X : " << axes[0] << std::endl;
		// std::cout << "Left  Y : " << axes[1] << std::endl;
		// std::cout << "Right X : " << axes[2] << std::endl;
		// std::cout << "Right X : " << axes[3] << std::endl;
		// std::cout << "L2      : " << axes[4] << std::endl;
		// std::cout << "R2      : " << axes[5] << std::endl;
	}
}


glm::vec3 Input::getAxisLS() {
	return this->axisLS;
}


glm::vec3 Input::getNormalLS() {
	return this->normalLS;
}
