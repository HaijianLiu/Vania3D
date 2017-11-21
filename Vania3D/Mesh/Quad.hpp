
#ifndef Quad_hpp
#define Quad_hpp

class Quad {
	friend class RenderPass;
	
private:
	unsigned int vao;

public:
	Quad();
	~Quad();

	void draw();
};

#endif /* Quad_hpp */
