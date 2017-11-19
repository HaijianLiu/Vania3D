
#ifndef RenderPass_hpp
#define RenderPass_hpp

struct FrameBuffer {
	unsigned int fbo;
	std::vector<unsigned int> textures;
};

class RenderPass {
	friend class Game;
	friend class Scene;

private:
	unsigned int vao;
	
	Shader* deferredPBR;
	FrameBuffer bufferG;

	void init(Shader* shader, unsigned int number);
	void render(RenderLayer* renderLayer, std::vector<GameObject*>* pointLights, GameObject* camera);
	// test
	void renderBounding(std::vector<MeshRenderer*>* renderQueue, GameObject* camera);

public:
	RenderPass();
	~RenderPass();

	void setActiveLightProbe(LightProbe* lightProbe);
	static FrameBuffer createFrameBuffer(unsigned int number);
};

#endif /* RenderPass_hpp */
