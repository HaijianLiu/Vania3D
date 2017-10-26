
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Animation::Animation() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Animation::~Animation() {
	deleteVector(this->keyframes);
}





Matrix4 Animation::getNodeTransformation(const Node<Matrix4>* node, float animationTimeInTicks) const {
	const Keyframe* keyframe = this->findKeyframe(node->name);

	if (keyframe) {
		// Interpolate scaling and generate scaling transformation matrix
		Vector3 Scaling;
		CalcInterpolatedScaling(Scaling, animationTimeInTicks, keyframe);
		Matrix4 ScalingM;
		ScalingM.setScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		Quaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, animationTimeInTicks, keyframe);
		// aiQuaternion qt = RotationQ.getAissmp();
		Matrix4 RotationM = Matrix4(RotationQ.getAissmp().GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		Vector3 Translation;
		CalcInterpolatedPosition(Translation, animationTimeInTicks, keyframe);
		Matrix4 TranslationM;
		TranslationM.setTranslationTransform(Translation.x, Translation.y, Translation.z);

		// Combine the above transformations
		Matrix4 nodeTransformation = TranslationM * RotationM * ScalingM;
		return nodeTransformation;
	}

	return node->data;
}





const Keyframe* Animation::findKeyframe(const std::string nodeName) const {
    for (unsigned int i = 0 ; i < this->keyframes.size() ; i++) {
        const Keyframe* keyframe = this->keyframes[i];

			if (keyframe->nodeName == nodeName) {
            return keyframe;
        }
    }

    return nullptr;
}


uint Animation::FindPosition(float animationTimeInTicks, const Keyframe* keyframe) const {
    for (unsigned int i = 0 ; i < keyframe->positionKeys.size() - 1 ; i++) {
        if (animationTimeInTicks < keyframe->positionKeys[i + 1].time) {
            return i;
        }
    }

    assert(0);

    return 0;
}


uint Animation::FindRotation(float animationTimeInTicks, const Keyframe* keyframe) const {
    assert(keyframe->rotationKeys.size() > 0);

    for (unsigned int i = 0 ; i < keyframe->rotationKeys.size() - 1 ; i++) {
        if (animationTimeInTicks < keyframe->rotationKeys[i + 1].time) {
            return i;
        }
    }

    assert(0);

    return 0;
}


uint Animation::FindScaling(float animationTimeInTicks, const Keyframe* keyframe) const {
    assert(keyframe->scalingKeys.size() > 0);

    for (unsigned int i = 0 ; i < keyframe->scalingKeys.size() - 1 ; i++) {
        if (animationTimeInTicks < keyframe->scalingKeys[i + 1].time) {
            return i;
        }
    }

    assert(0);

    return 0;
}


void Animation::CalcInterpolatedPosition(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe) const {
    if (keyframe->positionKeys.size() == 1) {
        Out = keyframe->positionKeys[0].value;
        return;
    }

    uint PositionIndex = FindPosition(animationTimeInTicks, keyframe);
    uint NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < keyframe->positionKeys.size());
    float DeltaTime = keyframe->positionKeys[NextPositionIndex].time - keyframe->positionKeys[PositionIndex].time;
    float Factor = (animationTimeInTicks - keyframe->positionKeys[PositionIndex].time) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    Vector3 Start = keyframe->positionKeys[PositionIndex].value;
    Vector3 End = keyframe->positionKeys[NextPositionIndex].value;
    Vector3 Delta = End - Start;
    Out = Start + Factor * Delta;
}


void Animation::CalcInterpolatedRotation(Quaternion& Out, float animationTimeInTicks, const Keyframe* keyframe) const {
	// we need at least two values to interpolate...
    if (keyframe->rotationKeys.size() == 1) {
        Out = keyframe->rotationKeys[0].value;
        return;
    }

    uint RotationIndex = FindRotation(animationTimeInTicks, keyframe);
    uint NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < keyframe->rotationKeys.size());
    float DeltaTime = keyframe->rotationKeys[NextRotationIndex].time - keyframe->rotationKeys[RotationIndex].time;
    float Factor = (animationTimeInTicks - keyframe->rotationKeys[RotationIndex].time) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    aiQuaternion StartRotationQ = keyframe->rotationKeys[RotationIndex].value.getAissmp();
    aiQuaternion EndRotationQ   = keyframe->rotationKeys[NextRotationIndex].value.getAissmp();
	aiQuaternion result;
    aiQuaternion::Interpolate(result, StartRotationQ, EndRotationQ, Factor);
	Out = result;
   Out.normalize();
}


void Animation::CalcInterpolatedScaling(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe) const {
    if (keyframe->scalingKeys.size() == 1) {
        Out = keyframe->scalingKeys[0].value;
        return;
    }

    uint ScalingIndex = FindScaling(animationTimeInTicks, keyframe);
    uint NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < keyframe->scalingKeys.size());
    float DeltaTime = keyframe->scalingKeys[NextScalingIndex].time - keyframe->scalingKeys[ScalingIndex].time;
    float Factor = (animationTimeInTicks - keyframe->scalingKeys[ScalingIndex].time) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    Vector3 Start = keyframe->scalingKeys[ScalingIndex].value;
    Vector3 End   = keyframe->scalingKeys[NextScalingIndex].value;
    Vector3 Delta = End - Start;
    Out = Start + Factor * Delta;
}
