
#ifndef ShadowMapping_hpp
#define ShadowMapping_hpp

class ShadowMapping {
private:
	friend class Game;
	friend class RenderPass;
	friend class Scene;
	friend class MeshRenderer;

	Shader* shader;
	float retina;
	unsigned int fbo, depthMap, size;

	void init(Shader* shader, unsigned int size);
	void render(std::vector<MeshRenderer*>* shadowQueue);

public:
	// uniform
	glm::mat4 projection, view, lightSpace;

	// shadow mapping properties
	GameObject* target;
	glm::vec3 lightPositionOffset;
	float range, nearPlane, farPlane;

	ShadowMapping();
	~ShadowMapping();
};

#endif /* ShadowMapping_hpp */
