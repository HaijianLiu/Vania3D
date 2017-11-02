
#ifndef Camera_hpp
#define Camera_hpp

class Camera {
private:
	// Camera Attributes
	float field; // Degree° Field of View
	float ratio;
	float rangeStart;
	float rangeEnd;
	// Moment options
	float moveSpeed;
	float rotateSensitivity;

	// Eular Angles
	float yaw;
	float pitch;

public:
	// camera transform
	glm::vec3 position;
	glm::vec3 cameraFront, cameraUp, cameraRight;
	glm::vec3 worldFront, worldUp, worldRight;
	// target
	Transform* target;
	glm::vec3 offset;

	// uniforms
	glm::mat4 projection;
	glm::mat4 view;

	Camera();
	~Camera();

	void update();


	// for temp
	glm::vec3 offsetFromTarget;
	glm::quat offsetRotation = glm::quat(1,0,0,0);

	void rotate(float radiansHorizonal, float radiansVertical);
	void zoom(float distance);
};

#endif /* Camera_hpp */
