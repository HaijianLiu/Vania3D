
#ifndef GameObject_hpp
#define GameObject_hpp

class GameObject {
private:
	std::unordered_map<std::type_index, Component*> components;

public:
	GameObject();
	virtual ~GameObject();

	void start();
	void update();

	template <typename T> T* addComponent();
	template <typename T> T* getComponent();
};

#endif /* GameObject_hpp */
