
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
GameObject::GameObject() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
GameObject::~GameObject() {
	deleteMap(this->components);
}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void GameObject::start() {
	for (const auto & conponent : this->components) {
		conponent.second->start();
	}
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void GameObject::update() {
	for (const auto & conponent : this->components) {
		conponent.second->update();
	}
}


/*------------------------------------------------------------------------------
< add component >
------------------------------------------------------------------------------*/
template <typename T> T* GameObject::addComponent() {
	auto component = new T();
	component->gameObject = this;
	this->components[typeid(T)] = component;
	return *component;
}


/*------------------------------------------------------------------------------
< get component >
------------------------------------------------------------------------------*/
template <typename T> T* GameObject::getComponent() {
	auto iter = components.find(typeid(T));
	if (iter != std::end(this->components)) {
		return dynamic_cast<T*>(iter->second);
	}
	return nullptr;
}
