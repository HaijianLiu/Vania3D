
#ifndef GameObject_hpp
#define GameObject_hpp

class GameObject {
	friend class Scene;
	friend class MeshRenderer;

private:
	Scene* scene;
	std::unordered_map<std::type_index, Component*> components;

	void start();
	void update();

public:
	bool staticObject = true;

	GameObject();
	virtual ~GameObject();

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
