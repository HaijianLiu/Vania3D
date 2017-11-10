
#ifndef ShadowMapping_hpp
#define ShadowMapping_hpp

class ShadowMapping {
private:
	float retina;

public:
	unsigned int fbo, depthMap, size;
    
    Shader* shader;
	
	// shadow mapping properties
	glm::vec3 lightPosition = glm::vec3(-10, 10, -1);
	glm::vec3 lightTarget = glm::vec3(0, 0, 0);
	float range = 100, nearPlane = 0, farPlane = 20;
	
	glm::mat4 projection, view, lightSpace;
	
	ShadowMapping();
	~ShadowMapping();
	
	void init(Shader* shader, unsigned int size);
	void begin();
	void update();
	void end();
};

#endif /* ShadowMapping_hpp */
