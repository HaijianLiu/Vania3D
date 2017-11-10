
#ifndef ShadowMapping_hpp
#define ShadowMapping_hpp

class ShadowMapping {
private:
	float retina;

public:
	unsigned int fbo, depthMap, size;
	
	ShadowMapping();
	~ShadowMapping();
	
	void init(unsigned int size);
	void begin();
	void end();
	void render();
};

#endif /* ShadowMapping_hpp */
