
#ifndef MeshRenderer_hpp
#define MeshRenderer_hpp

class MeshRenderer {
private:
	Material* material;
	LightProbe* lightProbe = nullptr;

public:
	MeshRenderer(Material* material);
	~MeshRenderer();

	void addLightProbe(LightProbe* lightProbe);
};

#endif /* MeshRenderer_hpp */
