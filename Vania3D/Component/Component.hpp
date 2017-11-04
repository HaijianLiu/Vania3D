
#ifndef Component_hpp
#define Component_hpp

class Component {
private:
	friend class GameObject;
	GameObject* gameObject;

public:
	Component();
	virtual ~Component();

	virtual void start();
	virtual void update();

	GameObject* getGameObject();
};

#endif /* Component_hpp */
