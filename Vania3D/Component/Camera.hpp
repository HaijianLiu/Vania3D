
#ifndef Camera_hpp
#define Camera_hpp

class Camera : public Component {
private:
	// camera Attributes
	float field; // degree Field of View
	float ratio;
	float rangeStart;
	float rangeEnd;
	// moment options
	float moveSpeed;
	float rotateSensitivity;

public:
	// camera transform
	glm::vec3 position;
	glm::vec3 cameraFront, cameraUp, cameraRight;
	glm::vec3 worldFront, worldUp, worldRight;
	// target
	Transform* target;
	glm::vec3 offset; // camera target offset from target

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
