
#ifndef RenderLayer_hpp
#define RenderLayer_hpp

struct MeshRenderData {
	Transform* transform;
	Model* model;
	unsigned int meshIndex;
};

struct MaterialLayer {
	std::vector<MeshRenderData> meshRenderDatas;
};

class ShaderLayer {
private:
	Shader* shader;
	std::unordered_map<Material*, MaterialLayer*> materialLayers;
	std::unordered_map<Material*, MaterialLayer*> materialLayersTwoSides;

public:
	ShaderLayer();
	~ShaderLayer();

	void add(GameObject* gameObject, unsigned int meshIndex);
	void render(GameObject* camera);
};

class RenderLayer {
private:
	std::unordered_map<unsigned int, ShaderLayer*> shaderLayers;

public:
	RenderLayer();
	~RenderLayer();

	void add(GameObject* gameObject);
	void render(GameObject* camera);
};

#endif /* RenderQueue_hpp */
