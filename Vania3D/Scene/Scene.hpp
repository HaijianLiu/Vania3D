
#ifndef Scene_hpp
#define Scene_hpp

class Scene {
private:
	friend class SceneManager;
	friend class Level;

	// for scene manager
	bool started = false;
	void startScene();
	void updateScene();

	// game objects list
	std::vector<GameObject*> gameObjects; // component will be updated
	unsigned int index = 0;
	std::unordered_map<const char*, unsigned int> gameObjectsMapping;
	// render list
	std::vector<MeshRenderer*> shadowQueue; // a list for shadow casting game objects
	std::vector<MeshRenderer*> renderQueue; // a list for renderable game objects
	std::vector<GameObject*> pointLights; // a list for point lights to be rendered

protected:
	Game* game;
	GameObject* mainCamera;

public:
	Scene();
	virtual ~Scene();

	// for overload
	virtual void start() = 0;
	virtual void update();

	void addGameObject(const char* name, GameObject* gameObject);
	GameObject* getGameObject(const char* name);
};

#endif /* Scene_hpp */
