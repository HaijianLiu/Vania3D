
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

	unsigned int animation = 0;

public:
	Scene00();
	~Scene00();

	void start();
	void update();
};

#endif /* Scene00_hpp */
