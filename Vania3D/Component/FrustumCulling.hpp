
#ifndef FrustumCulling_hpp
#define FrustumCulling_hpp

struct Face {
	glm::vec3 point;
	glm::vec3 normal;
};



class FrustumCulling : public Component {
private:
	Transform* transform;
	Camera* camera;

	Face frustum[6];
	enum {NEAR = 0, FAR, TOP, BOTTOM, LEFT, RIGHT};

	float nearHeight, nearWidth, farHeight, farWidth;

	void start();
	void update();

public:
	FrustumCulling();
	~FrustumCulling();

	void cullingSphere(MeshRenderer* meshRenderer);
};

#endif /* FrustumCulling_hpp */
