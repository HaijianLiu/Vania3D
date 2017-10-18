
#ifndef Component_hpp
#define Component_hpp

class Component {
private:
	GameObject* gameObject;

public:
	virtual ~Component();

	virtual void start();
	virtual void update();

	GameObject* getGameObject();
};

#endif /* Component_hpp */
