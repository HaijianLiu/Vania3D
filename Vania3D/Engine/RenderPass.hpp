
#ifndef RenderPass_hpp
#define RenderPass_hpp

class RenderPass {
private:
	unsigned int fbo, vao;
	std::vector<unsigned int> pass;

public:
	Shader* shader;

	RenderPass();
	~RenderPass();

	void init(Shader* shader, unsigned int number);
	void render(std::vector<MeshRenderer*>* renderQueue, std::vector<GameObject*>* pointLights, GameObject* camera);

	void setActiveLightProbe(LightProbe* lightProbe);
};

#endif /* RenderPass_hpp */
