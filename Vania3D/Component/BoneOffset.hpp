
#ifndef BoneOffset_hpp
#define BoneOffset_hpp

class BoneOffset : public Component {
private:
	glm::mat4 boneOffset = glm::mat4(1);
	Model* model;

	void start();
	void update();

	void processNode(Node<Bone>* node);

public:
	GameObject* parent;
	unsigned int boneIndex = 0;
	std::string boneName;

	BoneOffset();
	~BoneOffset();
};

#endif /* BoneOffset_hpp */
