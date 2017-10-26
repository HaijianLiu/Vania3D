
#ifndef Animation_hpp
#define Animation_hpp

class Animation {
private:
	const Keyframe* findKeyframe(const std::string nodeName) const;
	void CalcInterpolatedScaling(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	void CalcInterpolatedRotation(Quaternion& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	void CalcInterpolatedPosition(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe) const;
	uint FindScaling(float animationTimeInTicks, const Keyframe* keyframe) const;
	uint FindRotation(float animationTimeInTicks, const Keyframe* keyframe) const;
	uint FindPosition(float animationTimeInTicks, const Keyframe* keyframe) const;

public:
	std::string name;
	float duration = 1.0;
	float ticksPerSecond = 25.0;
	std::vector<Keyframe*> keyframes;

	Animation();
	~Animation();

	Matrix4 getNodeTransformation(const Node<Matrix4>* node, float animationTimeInTicks) const;
};

#endif /* Animation_hpp */
