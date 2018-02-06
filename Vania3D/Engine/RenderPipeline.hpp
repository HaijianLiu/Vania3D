
#ifndef RenderPipeline_hpp
#define RenderPipeline_hpp

class RenderPipeline {
	friend class Game;
	friend class Scene;
	friend class LutController;

private:
	Quad* quad;
	Game* game;
	std::vector<RenderPass*> renderPasses;
	
	
	unsigned int currentLut;

	void render(RenderLayer* renderLayer, RenderLayer* fxLayer, std::vector<PointLight*>* pointLights, GameObject* camera);
	void renderBounding(std::vector<MeshRenderer*>* renderQueue, GameObject* camera);

public:
	void addRenderPass(RenderPass* renderPass);
	
	
	
	// render pass setting
	LightProbe* lightProbe;

	Shader* lutShader;

	RenderPipeline();
	~RenderPipeline();

	void start();
};

#endif /* RenderPipeline_hpp */
