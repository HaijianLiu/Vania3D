
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

	void start();
	void update();
};

#endif /* Game_hpp */
