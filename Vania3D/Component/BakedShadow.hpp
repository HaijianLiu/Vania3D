
#ifndef BakedShadow_hpp
#define BakedShadow_hpp

class BakedShadow : public Component {
	friend class Game;
	friend class Scene;
	
private:
	Shader* shader;
	FrameBuffer bakedShadowMap;

	void start();
	void compileShader(); // to be refactored
	
	void bake(std::vector<MeshRenderer*>* renderQueue);

public:
	float bakeSize = 1024;
	
	BakedShadow();
	~BakedShadow();
};

#endif /* BakedShadow_hpp */
