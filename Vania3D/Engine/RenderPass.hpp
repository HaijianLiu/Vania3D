
#ifndef RenderPass_hpp
#define RenderPass_hpp

struct FrameBuffer {
	unsigned int fbo;
	std::vector<unsigned int> passes;
};

class RenderPass {
	friend class Game;
	friend class Scene;

private:
	unsigned int vao;
	Shader* shader;
	FrameBuffer frameBuffer;
	FrameBuffer motionA, motionB, motionC;
	bool frameSwitch = false;

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
