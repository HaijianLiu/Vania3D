
#ifndef Transform_hpp
#define Transform_hpp

class Transform {
public:
	// original transformations
	glm::vec3 initFront = glm::vec3(0.0, 0.0, 1.0);
	// glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	// glm::vec3 right = glm::vec3(-1.0, 0.0, 0.0);
	// glm::vec3 position = glm::vec3(0.0);
	glm::vec3 initScaling = glm::vec3(1.0);
	// glm::quat rotation = glm::quat(1.0, 0.0, 0.0, 0.0);

	// current transformations
	glm::vec3 front = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 right = glm::vec3(-1.0, 0.0, 0.0);
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 scaling = glm::vec3(1.0);
	glm::quat rotation = glm::quat(1.0, 0.0, 0.0, 0.0);

	// matrix uniform
	glm::mat4 model = glm::mat4();

	Transform();
	~Transform();
	// update model matrix
	void update();
};

#endif /* Transform_hpp */
