
#ifndef Scene00_hpp
#define Scene00_hpp

class Scene00 : public Scene {
private:
	Camera* camera;

	// lights
	glm::vec3 lightPositions[4];
	glm::vec3 lightColors[4];

public:
	Scene00();
	~Scene00();

	void start();
	void update();
};

#endif /* Scene00_hpp */
