
#ifndef Particle_hpp
#define Particle_hpp

class Particle : public Component {
	friend class Emitter;
	
private:
	void start();
	void update();
	
	void scaleOverLife();
	
public:
	float lifeTime;
	glm::vec3 initScale, initVelocity;
	
	Particle();
	~Particle();
};

#endif /* Particle_hpp */
