
#ifndef Scene_hpp
#define Scene_hpp

class Scene {
private:
	/* data */

public:
	bool started = false;

	Scene();
	virtual ~Scene();

	virtual void start() = 0;
	virtual void update();
};

#endif /* Scene_hpp */
