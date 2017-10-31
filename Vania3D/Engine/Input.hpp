
#ifndef Input_hpp
#define Input_hpp

enum PS4_CONTROLLER {
	JOY_SQUARE, JOY_CROSS, JOY_CIRCLE, JOY_TRIANGLE,
	JOY_L1, JOY_R1, JOY_L2, JOY_R2,
	JOY_SHARE, JOY_OPTION,
	JOY_L3, JOY_R3, JOY_HOME, JOY_TOUCHPAD,
	JOY_UP, JOY_RIGHT, JOY_DOWN, JOY_LEFT
};

class Input {
private:
	int keys[GLFW_KEY_LAST] = {GLFW_RELEASE};
	int joyButtons[25] = {GLFW_RELEASE};
	int joyButtonCount, joyEvent = GLFW_DISCONNECTED, joyConnect = 0;

	Input();

	void updateJoystick(int button);

public:
	// controller properties
	float gravity; // speed in units per second that the axis falls toward neutral when no buttons are pressed
	float dead; // size of the analog dead zone, all analog device values within this range result map to neutral
	float sensitivity; // speed in units per second that the the axis will move toward the target value, this is for digital devices only
	float snap // if enabled, the axis value will reset to zero when pressing a button of the opposite direction

	static Input* getInstance();
	~Input();

	// joystick
	void joystickcallback(int joy, int event);
	bool getJoystickPress(int button);
	bool getJoystickTrigger(int button);


	const float* axis();

	// keyboard
	void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	bool getButtonPress(int button);
	bool getButtonTrigger(int button);
};

#endif /* Input_hpp */
