
#ifndef UVAnimation_hpp
#define UVAnimation_hpp

class UVAnimation : public Component {
private:
	void start();
	
public:
	float divideX, divideY, sampleTime;
	
	UVAnimation(float divideX, float divideY, float sampleTime);
	~UVAnimation();
};

#endif /* UVAnimation_hpp */
