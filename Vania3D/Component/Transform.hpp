
#ifndef Transform_hpp
#define Transform_hpp

class Transform {
public:
	// original transform properties
	glm::vec3 modelFront = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 modelUp = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 modelScale = glm::vec3(1.0);

	// current transform properties
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 scale = glm::vec3(1.0);
	glm::quat rotation = glm::quat(1.0, 0.0, 0.0, 0.0);

	// matrix uniform
	glm::mat4 model = glm::mat4();

	Transform();
	~Transform();

	// update model matrix before set to shader
	void update();

	// get methods
	glm::vec3 front();

	// menipulate methods
	void rotate(glm::vec3 direction, float radian);
};

#endif /* Transform_hpp */
