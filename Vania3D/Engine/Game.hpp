
#ifndef Game_hpp
#define Game_hpp

class Game {
private:
	Game();

public:
	static Game* getInstance();
	~Game();

	Window* window;
	Resources* resources;
	Input* input;
	SceneManager* sceneManager;
	RenderPass* renderPass;
	Time* time;

	void start();
	void update();
};

#endif /* Game_hpp */
