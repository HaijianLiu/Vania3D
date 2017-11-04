
#ifndef PointLight_hpp
#define PointLight_hpp

class PointLight : public Component {
public:
	glm::vec3 color = glm::vec3(1);
	float intensity = 1;

	PointLight();
	~PointLight();
};

#endif /* PointLight_hpp */
