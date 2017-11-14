
#ifndef Material_hpp
#define Material_hpp

class Material {
	friend class ShaderLayer;
	friend class RenderLayer;
	
private:
	Shader* shader;
	std::vector<Texture*> textures;
	std::unordered_map<const char*, unsigned int> texturesMapping;
	unsigned int index = 0;

public:
	bool twoSides = false;

	Material(Shader* shader);
	~Material();

	void addTexture(const char* textureUniform, Texture* texture);
	void setUniformLocations();
	void bindTextures();
};
#endif /* Material_hpp */
