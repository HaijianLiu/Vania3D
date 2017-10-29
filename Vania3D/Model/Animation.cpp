
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Animation::Animation() {
	this->keyframeNode = new Node<Keyframe*>();
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Animation::~Animation() {
	deleteVector(this->keyframes);
	delete this->keyframeNode;
}




void Animation::processNode(Node<Keyframe*>* keyframeNode, const Node<Matrix4>* node, const aiAnimation* aianimation) {
	// check every aiNodeAnim if mNodeName matches keyframeNode name
	// if match then copy the aiNodeAnim data (mNumPositionKeys mNumRotationKeys mNumScalingKeys) to keyframeNode data
	for (unsigned int i = 0; i < aianimation->mNumChannels; i++) {
		if (keyframeNode->name == aianimation->mChannels[i]->mNodeName.data) {
			keyframeNode->data = new Keyframe();
			// copy keyframes
			for (unsigned int j = 0; j < aianimation->mChannels[i]->mNumPositionKeys; j++) {
				VectorKey vectorKey;
				vectorKey.time = aianimation->mChannels[i]->mPositionKeys[j].mTime;
				vectorKey.value = aianimation->mChannels[i]->mPositionKeys[j].mValue;
				keyframeNode->data->positionKeys.push_back(vectorKey);
			}
			for (unsigned int j = 0; j < aianimation->mChannels[i]->mNumRotationKeys; j++) {
				QuaternionKey quaternionKey;
				quaternionKey.time = aianimation->mChannels[i]->mRotationKeys[j].mTime;
				quaternionKey.value = aianimation->mChannels[i]->mRotationKeys[j].mValue;
				keyframeNode->data->rotationKeys.push_back(quaternionKey);
			}
			for (unsigned int j = 0; j < aianimation->mChannels[i]->mNumScalingKeys; j++) {
				VectorKey vectorKey;
				vectorKey.time = aianimation->mChannels[i]->mScalingKeys[j].mTime;
				vectorKey.value = aianimation->mChannels[i]->mScalingKeys[j].mValue;
				keyframeNode->data->scalingKeys.push_back(vectorKey);
			}
			break;
		}
		else {
			keyframeNode->data = nullptr;
		}
	}

	// check children nodes
	for (unsigned int i = 0; i < node->children.size(); i++) {
		keyframeNode->children.push_back(new Node<Keyframe*>(node->children[i]->name));
		processNode(keyframeNode->children[i], node->children[i], aianimation);
	}
}

void Animation::copyNodeTree(const Node<Matrix4>* rootNode, const aiAnimation* aianimation) {
	this->keyframeNode->name = rootNode->name;
	this->processNode(this->keyframeNode, rootNode, aianimation);
}






void Animation::processPose(std::vector<Matrix4>& pose, Node<Keyframe*>* keyframeNode, const Node<Matrix4>* node, const std::unordered_map<std::string, Bone>* bones, Matrix4 parentTransformation, float animationTimeInTicks) {

	Matrix4 nodeTransformation;

	if (keyframeNode->data) {
		// Interpolate scaling and generate scaling transformation matrix
		Vector3 Scaling;
		CalcInterpolatedScaling(Scaling, animationTimeInTicks, keyframeNode->data);
		Matrix4 ScalingM;
		ScalingM.setScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		Quaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, animationTimeInTicks, keyframeNode->data);
		// aiQuaternion qt = RotationQ.getAissmp();
		Matrix4 RotationM = Matrix4(RotationQ.getAissmp().GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		Vector3 Translation;
		CalcInterpolatedPosition(Translation, animationTimeInTicks, keyframeNode->data);
		Matrix4 TranslationM;
		TranslationM.setTranslationTransform(Translation.x, Translation.y, Translation.z);

		// Combine the above transformations
		nodeTransformation = TranslationM * RotationM * ScalingM;
	}
	else {
		nodeTransformation = node->data;
	}

	Matrix4 globalTransformation = parentTransformation * nodeTransformation;

	// set pose
	auto it = bones->find(node->name);
	if (it != bones->end()) {
		pose[it->second.index] = globalTransformation * it->second.offset; // may multiply rootNode->data * before?
	}

	for (unsigned int i = 0 ; i < node->children.size() ; i++) {
		this->processPose(pose, keyframeNode->children[i], node->children[i], bones, globalTransformation, animationTimeInTicks);
	}
}


void Animation::updatePose(std::vector<Matrix4>& pose, const Node<Matrix4>* rootNode, const std::unordered_map<std::string, Bone>* bones, float timeInSeconds) {
	float timeInTicks = timeInSeconds * this->ticksPerSecond;
	float animationTimeInTicks = fmod(timeInTicks, this->duration);
	this->processPose(pose, this->keyframeNode->children[0], rootNode->children[0]->children[0], bones, Matrix4::identity(), animationTimeInTicks);
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
