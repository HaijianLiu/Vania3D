
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


	template <typename T> T* addComponent() {
		auto component = new T();
		component->gameObject = this;
		this->components[typeid(T)] = component;
		return component;
	}

	template <typename T> T* getComponent() {
		auto iter = components.find(typeid(T));
		if (iter != std::end(this->components))
			return dynamic_cast<T*>(iter->second);
		return nullptr;
	}
};

#endif /* GameObject_hpp */