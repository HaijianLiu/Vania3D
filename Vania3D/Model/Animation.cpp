
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Animation::Animation(const char* path) {
	this->load(path);
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Animation::~Animation() {
	delete this->keyframeNode;
}


/*------------------------------------------------------------------------------
< load animation >
loads a model with supported ASSIMP extensions from file and stores the resulting animation keyframes in a node tree.
------------------------------------------------------------------------------*/
void Animation::load(const char* path) {
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* aiscene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights);
	// check for errors
	if(!aiscene || !aiscene->mRootNode) {
		std::cout << "[ERROR] ASSIMP: " << importer.GetErrorString() << std::endl;
		return;
	}

	// copy aiAnimation
	if (aiscene->mNumAnimations == 1) {
		this->duration = aiscene->mAnimations[0]->mDuration;
		this->ticksPerSecond = aiscene->mAnimations[0]->mTicksPerSecond;
	}
	else {
		std::cout << "[ERROR] ASSIMP: wrong number of animations" << std::endl;
		return;
	}

	// process assimp root node recursively
	this->keyframeNode = new Node<Keyframe*>(aiscene->mRootNode->mName.data);
	this->keyframeNode->data = nullptr;
	this->processNode(this->keyframeNode, aiscene->mRootNode, aiscene);
}






void Animation::processNode(Node<Keyframe*>* keyframeNode, const aiNode* ainode, const aiScene* aiscene) {
	// check every aiNodeAnim if mNodeName matches keyframeNode name
	// if match then copy the aiNodeAnim data (mNumPositionKeys mNumRotationKeys mNumScalingKeys) to keyframeNode data
	for (unsigned int i = 0; i < aiscene->mAnimations[0]->mNumChannels; i++) {
		if (keyframeNode->name == aiscene->mAnimations[0]->mChannels[i]->mNodeName.data) {
			keyframeNode->data = new Keyframe();
			// copy keyframes
			for (unsigned int j = 0; j < aiscene->mAnimations[0]->mChannels[i]->mNumPositionKeys; j++) {
				VectorKey vectorKey;
				vectorKey.time = aiscene->mAnimations[0]->mChannels[i]->mPositionKeys[j].mTime;
				vectorKey.value = aiscene->mAnimations[0]->mChannels[i]->mPositionKeys[j].mValue;
				keyframeNode->data->positionKeys.push_back(vectorKey);
			}
			for (unsigned int j = 0; j < aiscene->mAnimations[0]->mChannels[i]->mNumRotationKeys; j++) {
				QuaternionKey quaternionKey;
				quaternionKey.time = aiscene->mAnimations[0]->mChannels[i]->mRotationKeys[j].mTime;
				quaternionKey.value = aiscene->mAnimations[0]->mChannels[i]->mRotationKeys[j].mValue;
				keyframeNode->data->rotationKeys.push_back(quaternionKey);
			}
			for (unsigned int j = 0; j < aiscene->mAnimations[0]->mChannels[i]->mNumScalingKeys; j++) {
				VectorKey vectorKey;
				vectorKey.time = aiscene->mAnimations[0]->mChannels[i]->mScalingKeys[j].mTime;
				vectorKey.value = aiscene->mAnimations[0]->mChannels[i]->mScalingKeys[j].mValue;
				keyframeNode->data->scalingKeys.push_back(vectorKey);
			}
			break;
		}
		else {
			keyframeNode->data = nullptr;
		}
	}

	// check children nodes
	for (unsigned int i = 0; i < ainode->mNumChildren; i++) {
		keyframeNode->children.push_back(new Node<Keyframe*>(ainode->mChildren[i]->mName.data));
		keyframeNode->children[i]->data = nullptr;
		processNode(keyframeNode->children[i], ainode->mChildren[i], aiscene);
	}
}

//void Animation::copyNodeTree(const Node<Matrix4>* rootNode, const aiAnimation* aianimation) {
//    this->keyframeNode->name = rootNode->name;
//    this->processNode(this->keyframeNode, rootNode, aianimation);
//}






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

	for (unsigned int i = 0 ; i < keyframeNode->children.size() ; i++) {
		this->processPose(pose, keyframeNode->children[i], node->children[i], bones, globalTransformation, animationTimeInTicks);
	}
}


void Animation::updatePose(std::vector<Matrix4>& pose, const Node<Matrix4>* rootNode, const std::unordered_map<std::string, Bone>* bones, float timeInSeconds) {
	float timeInTicks = timeInSeconds * this->ticksPerSecond;
	float animationTimeInTicks = fmod(timeInTicks, this->duration);

	// for unreal engine
	// this->processPose(pose, this->keyframeNode->children[0], rootNode->children[0]->children[0], bones, Matrix4::identity(), animationTimeInTicks);

	// for mixamo
	this->processPose(pose, this->keyframeNode, rootNode, bones, Matrix4::identity(), animationTimeInTicks);
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
