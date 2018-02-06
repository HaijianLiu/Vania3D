
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
	friend class RenderPipeline;
	
private:
	std::string name;
	Shader* shader;
	FrameBuffer frameBuffer;
	std::vector<GLint> colorAttachments;
	std::vector<GLenum> depthAttachments;
	
	static unsigned int createColorAttachment(GLenum attachment, GLint internalFormat);
	static void createDepthAttachment(GLenum internalformat);
	static void drawBuffers(GLsizei n);
	
	void start();

public:
	RenderPass(std::string name);
	~RenderPass();
	
	void addColorAttachment(GLint internalFormat);
	void addDepthAttachment(GLenum internalFormat);
};


#endif /* RenderPass_hpp */
