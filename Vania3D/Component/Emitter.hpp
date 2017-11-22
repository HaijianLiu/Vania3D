
#ifndef Emitter_hpp
#define Emitter_hpp

class Emitter : public Component {
private:
	std::vector<GameObject*> particles;
	
	void start();
	void update();
	
public:
	unsigned int maxParticles = 10;
	float lifeTime;
	float initialLocation, initialSize, initialVelocity;
	float sizeByLife, velocityByLife, colorOverTime;
	Material* material;
	
	void createParticles(const char* name, Scene* scene);
	
	Emitter();
	~Emitter();
};

#endif /* Emitter_hpp */
