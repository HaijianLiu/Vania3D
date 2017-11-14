
#ifndef Camera_hpp
#define Camera_hpp

class Camera : public Component {
	friend class MeshRenderer;

private:
	// camera attributes
	float field = 45;
	float ratio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;;
	float rangeStart = 0.1;
	float rangeEnd = 50;

	// uniforms
	glm::mat4 projection;
	glm::mat4 view;

	void update();

public:
	// properties
	Transform* target;
	// camera angles
	glm::vec3 front, up, right;

	Camera();
	~Camera();

	void setUniforms(Shader* shader);
};

#endif /* Camera_hpp */
