
#ifndef Scene01_hpp
#define Scene01_hpp

class Scene01 : public Scene {
public:
	unsigned int depthMapFBO;
	unsigned int depthMap;
	
	Scene01();
	~Scene01();

	void start();
	void update();
};

#endif /* Scene01_hpp */
