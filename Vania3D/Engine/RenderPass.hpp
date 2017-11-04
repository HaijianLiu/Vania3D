
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
	void begin();
	void end();
	void render();

	void setActiveLightProbe(LightProbe* lightProbe);
};

#endif /* RenderPass_hpp */
