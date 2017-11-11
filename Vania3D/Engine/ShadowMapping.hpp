
#ifndef ShadowMapping_hpp
#define ShadowMapping_hpp

class ShadowMapping {
private:
	float retina;

public:
	unsigned int fbo, depthMap, size;
    
    Shader* shader;
	GameObject* target;
	
	// shadow mapping properties
	glm::vec3 lightPositionOffset;
	float range, nearPlane, farPlane;
	
	// uniform
	glm::mat4 projection, view, lightSpace;
	
	ShadowMapping();
	~ShadowMapping();
	
	void init(Shader* shader, unsigned int size);
	void render(std::vector<MeshRenderer*>* shadowQueue);
};

#endif /* ShadowMapping_hpp */
