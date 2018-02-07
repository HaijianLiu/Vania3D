
#ifndef RenderPass_hpp
#define RenderPass_hpp

struct FrameBuffer {
	unsigned int fbo;
	std::vector<unsigned int> textures;
};

class RenderPass {
	friend class RenderPipeline;
	
	std::string name;

	FrameBuffer frameBuffer;
	std::vector<GLint> colorAttachments;
	std::vector<GLenum> depthAttachments;
	
	static unsigned int createColorAttachment(GLenum attachment, GLint internalFormat);
	static void createDepthAttachment(GLenum internalformat);
	static void drawBuffers(GLsizei n);
	


public:
	RenderPass(std::string name);
	~RenderPass();
	
		void start();
		Shader* shader;
	
	void addColorAttachment(GLint internalFormat);
	void addDepthAttachment(GLenum internalFormat);
};


#endif /* RenderPass_hpp */
