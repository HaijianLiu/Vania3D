
#ifndef MeshRenderer_hpp
#define MeshRenderer_hpp

class MeshRenderer : public Component {
private:
	Material* material;
	LightProbe* lightProbe;

public:
	Model* model;
	
	Camera* camera;

	MeshRenderer();
	~MeshRenderer();

	void start();
	void update();

	void addModel(Model* model);
	void addMaterial(Material* material);
	void addLightProbe(LightProbe* lightProbe);
};

#endif /* MeshRenderer_hpp */
