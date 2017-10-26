
#ifndef Animation_hpp
#define Animation_hpp

class Animation {
public:
	std::string name;
	float duration = 0.0;
	float ticksPerSecond = 25.0;
	std::vector<Keyframe*> keyframes;

	Animation();
	~Animation();

	const Keyframe* findKeyframe(const std::string nodeName) const;
};

#endif /* Animation_hpp */
