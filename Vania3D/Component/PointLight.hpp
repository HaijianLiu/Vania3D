
#ifndef PointLight_hpp
#define PointLight_hpp

class PointLight : public Component {
	friend class FrustumCulling;
	
private:
	bool culling = false;
	float radius;

	void start();

public:
	glm::vec3 color = glm::vec3(1);
	float intensity = 1;
	bool distanceCulling = true;

	PointLight();
	~PointLight();
};

#endif /* PointLight_hpp */
