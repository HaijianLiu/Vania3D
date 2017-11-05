
#ifndef Camera_hpp
#define Camera_hpp

class Camera : public Component {
private:
	// camera attributes
	float field = 45;
	float ratio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;;
	float rangeStart = 0.1;
	float rangeEnd = 100;

public:
	// camera angle
	glm::vec3 front, up, right;

	// target
	Transform* target;

	// uniforms
	glm::mat4 projection;
	glm::mat4 view;

	Camera();
	~Camera();

	void update();

	// for temp
	glm::vec3 offsetFromTarget; // camera offset from target's offset

	void rotate(float radiansHorizonal, float radiansVertical);
	void zoom(float distance);
};

#endif /* Camera_hpp */
