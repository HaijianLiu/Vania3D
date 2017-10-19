
#ifndef SceneManager_hpp
#define SceneManager_hpp

class SceneManager {
private:
	const char* currentScene;
	std::unordered_map<const char*, Scene*> scenes;

	void start();

public:
	SceneManager();
	~SceneManager();

	void update();
	void add(const char* name, Scene* scene);
	void setActiveScene(const char* name);
};

#endif /* SceneManager_hpp */
