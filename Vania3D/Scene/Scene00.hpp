
#ifndef Scene00_hpp
#define Scene00_hpp

class Scene00 : public Scene {
private:
	Camera* camera;
	
	CameraController* cameraController;

	// lights
	glm::vec3 lightPositions[4];
	glm::vec3 lightColors[4];

	// player
//	unsigned int animation = 0;
//	float lastAttack = 0;
//	Transform* transform;

public:
	Scene00();
	~Scene00();

	void start();
	void update();
};

#endif /* Scene00_hpp */
