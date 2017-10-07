
#ifndef Game_hpp
#define Game_hpp

class Game {
private:
	// std::vector<GameObject*> gameObjects;
	// std::vector<BoxCollider*> colliders;

public:
	// Timer* timer;
	Camera* camera;
	Resources* resources;
	// SceneManager* sceneManager;

	// Player* player;
	// std::map<const char*, GameObject*> publicObjects;
	// std::map<const char*, ParticleSystem*> particleSystems;

	Game();
	~Game();

	void start();
	void update();

	// std::vector<GameObject*>* getGameObjects();
	// std::vector<BoxCollider*>* getColliders();
	// std::vector<GameObject*> copyGameObjects();
	// std::vector<BoxCollider*> copyColliders();
};

#endif /* Game_hpp */
