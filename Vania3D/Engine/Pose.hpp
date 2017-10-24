
#ifndef Pose_hpp
#define Pose_hpp

class Pose {
private:
	std::vector<Bone*> bones;

public:
	Pose();
	~Pose();
};

#endif /* Pose_hpp */
