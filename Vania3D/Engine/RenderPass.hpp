
#ifndef RenderPass_hpp
#define RenderPass_hpp

struct FrameBuffer {
	unsigned int fbo;
	std::vector<unsigned int> textures;
};

class RenderPass {
	friend class Game;
	friend class Scene;
	friend class LutController;

private:
	Quad* quad;
	FrameBuffer deferredPass, fxPass, ambientPass, lightingPass, shadowPass, ssaoPass, combinePass;
	unsigned int currentLut;

	void render(RenderLayer* renderLayer, RenderLayer* fxLayer, std::vector<PointLight*>* pointLights, GameObject* camera);
	void renderBounding(std::vector<MeshRenderer*>* renderQueue, GameObject* camera);

	static unsigned int createColorAttachment(GLenum attachment, GLint internalFormat);
	static void createDepthAttachment(GLenum internalformat);
	static void drawBuffers(GLsizei n);

public:
	// render pass setting
	LightProbe* lightProbe;
	Shader* ambientShader;
	Shader* lightingShader;
	Shader* shadowShader;
	Shader* ssaoShader;
	Shader* combineShader;
	Shader* lutShader;

	RenderPass();
	~RenderPass();

	void start();
};

#endif /* RenderPass_hpp */
