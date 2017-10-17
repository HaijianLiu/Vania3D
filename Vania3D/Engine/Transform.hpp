
#ifndef Transform_hpp
#define Transform_hpp

class Transform {
private:
	Window* window;

public:
	// Transform
	glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 scale = glm::vec3(1.0f,1.0f,1.0f);
	glm::vec3 rotation = glm::vec3(0.0f,0.0f,0.0f);

	// matrix uniform
	glm::mat4 model = glm::mat4();

	Transform();
	~Transform();

	void update();
};

#endif /* Transform_hpp */
