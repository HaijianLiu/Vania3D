
#ifndef Emitter_hpp
#define Emitter_hpp

class Emitter : public Component {
private:
	float lastSpawnTime = 0;
	
	std::vector<Particle*> particles;
	
	void start();
	void update();

	void activeParticle();
	
public:
	Material* material;
	
	unsigned int maxParticles = 10;
	float spawnTime = 0.1;
	
	void createParticles(const char* name, Scene* scene);

	Emitter();
	~Emitter();
};

#endif /* Emitter_hpp */
