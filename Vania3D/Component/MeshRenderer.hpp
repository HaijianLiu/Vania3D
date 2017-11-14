
#ifndef MeshRenderer_hpp
#define MeshRenderer_hpp

class MeshRenderer : public Component {
	friend class ShadowMapping;

private:
	GameObject* camera;

	void start();
	void renderShadow();

public:
	// render targets
	LightProbe* lightProbe;
	// ....... add reflection probes here
	std::vector<Material*> materials;
	Model* model;

	// properties
	bool castShadow = false;
	bool render = true;
	bool lightmapStatic = true;

	MeshRenderer();
	~MeshRenderer();
};

#endif /* MeshRenderer_hpp */
