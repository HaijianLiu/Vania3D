
#ifndef MeshRenderer_hpp
#define MeshRenderer_hpp

class MeshRenderer : public Component {
private:
	LightProbe* lightProbe;

public:
	Material* material;
	Model* model;

	GameObject* camera;
	
	bool castShadow = false;

	MeshRenderer();
	~MeshRenderer();

	void start();
	void update();
    
    void drawShadow();

	void addModel(Model* model);
	void addMaterial(Material* material);
	void addLightProbe(LightProbe* lightProbe);
};

#endif /* MeshRenderer_hpp */
