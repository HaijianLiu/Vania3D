
#ifndef Texture_hpp
#define Texture_hpp

class Texture {
private:

public:
	Texture(const char* path);
	~Texture();

	unsigned int id;

	// static load functions
	static unsigned int loadTexture(const char* path);
};

#endif /* Texture_hpp */
