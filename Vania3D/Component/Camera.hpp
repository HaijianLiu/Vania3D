
#ifndef Camera_hpp
#define Camera_hpp

class Camera : public Component {
private:
	// camera Attributes
	float field = 45;
	float ratio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;;
	float rangeStart = 0.1;
	float rangeEnd = 100;

public:
	// Camera Position
	glm::vec3 cameraFront = glm::vec3(0.0,0.0,-1.0);
	glm::vec3 cameraUp = glm::vec3(0.0,1.0,0.0);
	glm::vec3 cameraRight = glm::vec3(1.0,0.0,0.0);

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
