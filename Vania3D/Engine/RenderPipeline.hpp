
#ifndef RenderPipeline_hpp
#define RenderPipeline_hpp

class RenderPipeline {
	friend class Game;
	friend class Scene;
	friend class LutController;

	Quad* quad;
	Game* game;
	std::vector<RenderPass*> renderPasses;

	void render(Scene* scene);
	void renderBounding(std::vector<MeshRenderer*>* renderQueue, GameObject* camera);

public:
	void addRenderPass(RenderPass* renderPass);
	
	// render pass setting
	LightProbe* lightProbe;

	RenderPipeline();
	~RenderPipeline();

	void start();
};

#endif /* RenderPipeline_hpp */
