
#ifndef ShadowMapping_hpp
#define ShadowMapping_hpp

class ShadowMapping {
private:


public:
		unsigned int fbo, depthMap;
	
	ShadowMapping();
	~ShadowMapping();
	
	void init(unsigned int size);
};

#endif /* ShadowMapping_hpp */
