
#ifndef OffsetTransform_hpp
#define OffsetTransform_hpp

class Offset : public Component {
public:
	Transform* parent;

	// offset
	glm::vec3 offsetPosition = glm::vec3(0);

	Offset();
	~Offset();

	void update();
};

#endif /* OffsetTransform_hpp */
