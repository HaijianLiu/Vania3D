
#ifndef BoneOffset_hpp
#define BoneOffset_hpp

class BoneOffset : public Component {
private:
	void update();

public:
	// target
	Transform* parent;
	// properties
	glm::vec3 offsetPosition = glm::vec3(0);

	BoneOffset();
	~BoneOffset();
};

#endif /* BoneOffset_hpp */
