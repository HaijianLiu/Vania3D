
#ifndef Scene_hpp
#define Scene_hpp

class Scene {
private:
	friend class SceneManager;
	// for scene manager
	void startScene();
	void updateScene();

	std::vector<GameObject*> gameObjects; // component will be updated
	unsigned int index = 0;
	std::unordered_map<const char*, unsigned int> gameObjectsMapping;

	std::vector<MeshRenderer*> shadowQueue; // a list for shadow casting game objects
	std::vector<MeshRenderer*> renderQueue; // a list for renderable game objects
	std::vector<GameObject*> pointLights; // a list for point lights to be rendered

public:
	GameObject* mainCamera;

	Game* game;

	bool started = false;

	Scene();
	virtual ~Scene();

	// for overload
	virtual void start() = 0;
	virtual void update();

	void addGameObject(const char* name, GameObject* gameObject);
	void addCamera(GameObject* camera);

	GameObject* getGameObject(const char* name);
};

#endif /* Scene_hpp */
