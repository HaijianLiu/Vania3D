
#ifndef RenderPass_hpp
#define RenderPass_hpp

struct FrameBuffer {
	unsigned int fbo;
	std::vector<unsigned int> textures;
};

struct TextureAttachment {
	GLenum unit;
	GLenum target;
	GLuint texture;
};

class RenderPass {
	friend class RenderPipeline;

	std::string name;
	Shader* shader;
	FrameBuffer frameBuffer;
	std::vector<GLint> colorAttachments;
	std::vector<GLenum> depthAttachments;
	std::vector<TextureAttachment> dynamicTextureAttachments;

	static unsigned int createColorAttachment(GLenum attachment, GLint internalFormat);
	static void createDepthAttachment(GLenum internalformat);
	static void drawBuffers(GLsizei n);
	
	void start();
	void update();

public:
	RenderPass(std::string name);
	~RenderPass();
	
	void addColorAttachment(GLint internalFormat);
	void addDepthAttachment(GLenum internalFormat);
	void addShader(Shader* shader);
	void addDynamicTextureAttachment(GLenum unit, GLenum target, GLuint texture); // texture will be updated per frame

	unsigned int getTexture(unsigned int index);
};


#endif /* RenderPass_hpp */
