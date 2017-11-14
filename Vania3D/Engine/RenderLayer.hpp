
#ifndef RenderLayer_hpp
#define RenderLayer_hpp

class Renderlayer {
private:
	Shader* shader;
	unsigned int fbo, vao;
	std::vector<unsigned int> pass;

	void init(Shader* shader, unsigned int number);
	void render(std::vector<MeshRenderer*>* renderQueue, std::vector<GameObject*>* pointLights, GameObject* camera);

public:
	Renderlayer();
	~Renderlayer();

	void setActiveLightProbe(LightProbe* lightProbe);
};

#endif /* RenderLayer_hpp */
