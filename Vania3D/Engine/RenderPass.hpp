
#ifndef RenderPass_hpp
#define RenderPass_hpp

class RenderPass {
private:
	Window* window;
	Resources* resources;

public:
	unsigned int fbo, vao;
	std::vector<unsigned int> pass;

	RenderPass();
	~RenderPass();

	void init(int number);
	void begin();
	void end();
	void render();
};

#endif /* RenderPass_hpp */
