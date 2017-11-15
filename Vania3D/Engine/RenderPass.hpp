
#ifndef RenderPass_hpp
#define RenderPass_hpp

class RenderPass {
	friend class Game;
	friend class Scene;

private:
	Shader* shader;
	unsigned int fbo, vao;
	std::vector<unsigned int> pass;

	void init(Shader* shader, unsigned int number);
	void render(RenderLayer* renderLayer, std::vector<GameObject*>* pointLights, GameObject* camera);
	// test
	void renderBounding(std::vector<MeshRenderer*>* renderQueue, GameObject* camera);

public:
	RenderPass();
	~RenderPass();

	void setActiveLightProbe(LightProbe* lightProbe);
};

#endif /* RenderPass_hpp */
