
#ifndef Component_hpp
#define Component_hpp

class Component {
	friend class GameObject;
	friend class RenderPass;
	friend class Scene;

protected:
	Game* game;

	virtual void start();
	virtual void update();

public:
	GameObject* gameObject;
	
	Component();
	virtual ~Component();
};

#endif /* Component_hpp */
