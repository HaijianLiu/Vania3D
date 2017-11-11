
#ifndef RenderPass_hpp
#define RenderPass_hpp

class RenderPass {
private:
	friend class Game;
	friend class Scene;

	Shader* shader;
	unsigned int fbo, vao;
	std::vector<unsigned int> pass;

	void init(Shader* shader, unsigned int number);
	void render(std::vector<MeshRenderer*>* renderQueue, std::vector<GameObject*>* pointLights, GameObject* camera);

public:
	RenderPass();
	~RenderPass();

	void setActiveLightProbe(LightProbe* lightProbe);
};

#endif /* RenderPass_hpp */
