
#ifndef Component_hpp
#define Component_hpp

class Component {
	friend class GameObject;
	friend class RenderPipeline;
	friend class Scene;
	friend class Shader;

protected:
	Game* game;
	GameObject* gameObject;

	virtual void start();
	virtual void update();

public:
	Component();
	virtual ~Component();
};

#endif /* Component_hpp */
