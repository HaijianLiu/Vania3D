
#ifndef Texture_hpp
#define Texture_hpp

enum TEXTURE_MODE {
	TEXTURE_MODE_NEAREST, TEXTURE_MODE_LINEAR
};

class Texture {
public:
	unsigned int id;

	Texture(const char* path);
	~Texture();

	static unsigned int loadTexture(const char* path);
	static void setTexMode(unsigned int & textureID, unsigned int type);
	static unsigned int resize(unsigned int textureID, int width, int height, unsigned int format);
	static unsigned int genNoiseTexture(int width, int height);
};

#endif /* Texture_hpp */
