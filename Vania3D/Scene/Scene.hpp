
#ifndef Scene_hpp
#define Scene_hpp

class Scene {
private:
	friend class SceneManager;
	std::vector<GameObject*> gameObjects;
	unsigned int index = 0;
	std::unordered_map<const char*, unsigned int> gameObjectsMapping;

public:
	bool started = false;

	Scene();
	virtual ~Scene();

	virtual void start() = 0;
	virtual void update();

	void addGameObject(const char* name, GameObject* gameObject);
	GameObject* getGameObject(const char* name);
};

#endif /* Scene_hpp */
