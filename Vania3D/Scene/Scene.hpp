
#ifndef Scene_hpp
#define Scene_hpp

class Scene {
	friend class SceneManager;
	friend class Level;
	friend class MeshRenderer;

private:
	bool started = false;

	// game objects list
	std::vector<GameObject*> gameObjects; // component will be updated
	unsigned int index = 0;
	std::unordered_map<const char*, unsigned int> gameObjectsMapping;
	// render list
	RenderLayer* renderLayer; // for final render
	std::vector<MeshRenderer*> shadowQueue; // a list for shadow casting game objects
	std::vector<MeshRenderer*> renderQueue; // a list for renderable game objects for some purpose
	std::vector<GameObject*> pointLights; // a list for point lights to be calculated
	
	void startScene();
	void updateScene();

protected:
	Game* game;
	GameObject* mainCamera;

	virtual void start() = 0;
	virtual void update();

public:
	Scene();
	virtual ~Scene();

	void addGameObject(const char* name, GameObject* gameObject);
	GameObject* getGameObject(const char* name);
};

#endif /* Scene_hpp */
