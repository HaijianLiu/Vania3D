
#ifndef BoneOffset_hpp
#define BoneOffset_hpp

class BoneOffset : public Component {
private:
	std::string boneName;
	
	void start();
	void update();

public:
	GameObject* parent;
	unsigned int boneIndex = 0;
	glm::mat4 boneOffset = glm::mat4(1);

	BoneOffset();
	~BoneOffset();
	
	void getBoneOffset();
	void processNode(Node<Bone>* node);
};

#endif /* BoneOffset_hpp */
