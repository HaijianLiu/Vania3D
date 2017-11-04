
#ifndef Scene_hpp
#define Scene_hpp

class Scene {
private:
	friend class SceneManager;

	std::vector<GameObject*> gameObjects; // component will be updated
	unsigned int index = 0;
	std::unordered_map<const char*, unsigned int> gameObjectsMapping;

	void updateRenderPass();

public:
	Game* game;
	Camera* camera;
	std::vector<GameObject*> lights; // component wont be updated

	bool started = false;

	Scene();
	virtual ~Scene();

	virtual void start() = 0;
	virtual void update();

	void addGameObject(const char* name, GameObject* gameObject);
	void addLight(GameObject* light);

	GameObject* getGameObject(const char* name);
};

#endif /* Scene_hpp */
