
#ifndef Scene00_hpp
#define Scene00_hpp

class Scene00 : public Scene {
private:
	Camera* camera;

	// lights
	glm::vec3 lightPositions[4];
	glm::vec3 lightColors[4];

	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// animation
	unsigned int animation = 0;
	float lastAttack = 0;

	// transform
	Transform* transform;
	// glm::vec3 front = glm::vec3(0.0, 0.0, 1.0);
	// glm::vec3 scaling;
	// glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
	// glm::quat lastRotation = glm::quat(1,0,0,0);


public:
	Scene00();
	~Scene00();

	void start();
	void update();
};

#endif /* Scene00_hpp */
