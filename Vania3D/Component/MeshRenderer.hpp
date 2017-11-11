
#ifndef MeshRenderer_hpp
#define MeshRenderer_hpp

class MeshRenderer : public Component {
private:
	LightProbe* lightProbe;
	// reflection probes

public:
	Material* material;
	Model* model;

	GameObject* camera;


	bool castShadow = false;
	bool render = true;
	bool lightmapStatic = true;

	MeshRenderer();
	~MeshRenderer();

	void start();

	void renderModel();
	void renderShadow();

	void addModel(Model* model);
	void addMaterial(Material* material);
	void addLightProbe(LightProbe* lightProbe);
};

#endif /* MeshRenderer_hpp */
