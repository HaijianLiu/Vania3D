
#ifndef MeshRenderer_hpp
#define MeshRenderer_hpp

enum RENDER_LAYER {
	RENDER_LAYER_DEFAULT, RENDER_LAYER_FX
};

class MeshRenderer : public Component {
	friend class ShadowMapping;
	friend class FrustumCulling;
	friend class MaterialLayer;

private:
	GameObject* camera;
	bool culling = false;

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
	bool lightmapStatic = true;
	int renderLayer = RENDER_LAYER_DEFAULT;

	MeshRenderer();
	~MeshRenderer();
};

#endif /* MeshRenderer_hpp */
