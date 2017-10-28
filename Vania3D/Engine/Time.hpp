
#ifndef Time_hpp
#define Time_hpp

class Time {
private:
	float lastTime = 0;

public:
	float currentTime = 0;
	float deltaTime = 0;

	Time();
	~Time();

	void start();
	void setTime();
	void update();
	bool checkFPS(int frameRate);
};
#endif /* Time_hpp */
