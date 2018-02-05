
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
	
	
public:
	RenderPass();
	~RenderPass();
};


#endif /* RenderPass_hpp */
