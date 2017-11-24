
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
	Quad* quad;
	
	Shader* finalShader;
	Shader* lightingShader;
	
	FrameBuffer deferredPass;
	FrameBuffer fxPass;
	FrameBuffer lightingPass;

	void init();
	void render(RenderLayer* renderLayer, RenderLayer* fxLayer, std::vector<PointLight*>* pointLights, GameObject* camera);
	void renderBounding(std::vector<MeshRenderer*>* renderQueue, GameObject* camera);
	
	static unsigned int createColorAttachment(GLenum attachment, GLint internalFormat);
	static void createDepthAttachment(GLenum internalformat);
	static void drawBuffers(GLsizei n);

public:
	RenderPass();
	~RenderPass();

	void setActiveLightProbe(LightProbe* lightProbe);
};

#endif /* RenderPass_hpp */
