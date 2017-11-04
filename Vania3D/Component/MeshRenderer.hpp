
#ifndef MeshRenderer_hpp
#define MeshRenderer_hpp

class MeshRenderer : public Component {
private:
	Material* material;
	LightProbe* lightProbe;

public:
	MeshRenderer();
	~MeshRenderer();

	void start();
	void update();

	void addMaterial(Material* material);
	void addLightProbe(LightProbe* lightProbe);
};

#endif /* MeshRenderer_hpp */
