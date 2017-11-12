
#ifndef Transform_hpp
#define Transform_hpp

class Transform : public Component {
	friend class Level;
	friend class Map;
	friend class MeshRenderer;

private:
	// uniform
	glm::mat4 model = glm::mat4();
	bool init = true; // update model uniform in start function, for level class to set model uniform directly

	void start();
	void update();

public:
	// original transform properties
	glm::vec3 modelScale = glm::vec3(1.0);
	// current transform properties
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 scale = glm::vec3(1.0);
	glm::quat rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
	// component properties
	bool kinematic = true; // default transform setting wont update model matrix uniform

	Transform();
	~Transform();

	// get methods
	glm::vec3 front();

	// menipulate methods
	void rotate(glm::vec3 direction, float radian);
};

#endif /* Transform_hpp */
