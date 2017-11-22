
#ifndef Emitter_hpp
#define Emitter_hpp

class Emitter : public Component {
private:
	float lastSpawnTime = 0;
	
	std::vector<GameObject*> particles;
	
	void start();
	void update();
	
	GameObject* activeParticle();
	
public:
	Material* material;
	
	unsigned int maxParticles = 10;
	float spawnTime = 0.1;
	float lifeTime = 1;
	
	float initialLocation, initialSize, initialVelocity;
	
	float sizeByLife, velocityByLife, colorOverTime;
	

	
	void createParticles(const char* name, Scene* scene);

	
	Emitter();
	~Emitter();
};

#endif /* Emitter_hpp */
