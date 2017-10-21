
#ifndef Material_hpp
#define Material_hpp

class Material {
private:
	Shader* shader;
	std::map<const char*, Texture*> textures;

public:
	Material(Shader* shader);
	~Material();

	void addTexture(const char* textureUniform, Texture* texture);
};
#endif /* Material_hpp */
