
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
	this->keyframeNode = new Node<Keyframe>(aiscene->mRootNode->mName.data);
	this->processNode(this->keyframeNode, aiscene->mRootNode, aiscene);
}


/*------------------------------------------------------------------------------
< process keyframe node >
check every aiNodeAnim if mNodeName matches keyframeNode name
if match then copy the aiNodeAnim data (mPositionKeys mRotationKeys mScalingKeys) to keyframeNode data
------------------------------------------------------------------------------*/
void Animation::processNode(Node<Keyframe>* keyframeNode, const aiNode* ainode, const aiScene* aiscene) {
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
	}

	// check children nodes
	for (unsigned int i = 0; i < ainode->mNumChildren; i++) {
		keyframeNode->children.push_back(new Node<Keyframe>(ainode->mChildren[i]->mName.data));
		processNode(keyframeNode->children[i], ainode->mChildren[i], aiscene);
	}
}





void Animation::processPose(std::vector<Matrix4>& pose, Node<Keyframe>* keyframeNode, const Node<Bone>* node, Matrix4 parentTransformation) {

	Matrix4 nodeTransformation;

	if (keyframeNode->data) {
		// Interpolate scaling and generate scaling transformation matrix
		Vector3 scaling = calcInterpolatedScaling(keyframeNode->data);
		Matrix4 scalingMatrix;
		scalingMatrix.setScaleTransform(scaling.x, scaling.y, scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		Quaternion rotation = calcInterpolatedRotation(keyframeNode->data);
		Matrix4 rotationMatrix = Matrix4(rotation.getAissmp().GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		Vector3 translation = calcInterpolatedPosition(keyframeNode->data);
		Matrix4 translationMatrix;
		translationMatrix.setTranslationTransform(translation.x, translation.y, translation.z);

		// Combine the above transformations
		nodeTransformation = translationMatrix * rotationMatrix * scalingMatrix;
	}
	else {
		nodeTransformation = node->data->nodeTransformation;
	}

	Matrix4 globalTransformation = parentTransformation * nodeTransformation;






	// animation blending
//	float blendFactor = (timeInSeconds - lastStartTimeInSeconds) / this->blendTimeInSeconds;
//	blendFactor > 1.0 ? blendFactor : 1.0;
//
//	aiMatrix4x4 x1, x2;
//	x1 = x1 - x2;




	// set pose
	if(node->data->haveBone) {
		pose[node->data->index] = globalTransformation * node->data->offset;
	}

	for (unsigned int i = 0 ; i < keyframeNode->children.size() ; i++) {
		this->processPose(pose, keyframeNode->children[i], node->children[i], globalTransformation);
	}
}


void Animation::updatePose(std::vector<Matrix4>& pose, const Node<Bone>* rootNode, float timeInSeconds) {
	timeInSeconds = timeInSeconds - lastStartTimeInSeconds; // animation always starts from the beginning
	float timeInTicks = timeInSeconds * this->ticksPerSecond;
	this->animationTimeInTicks = fmod(timeInTicks, this->duration);

	/* for mixamo models */
	this->processPose(pose, this->keyframeNode, rootNode, Matrix4::identity());
	/* for unreal engine models */
	// this->processPose(pose, this->keyframeNode->children[0], rootNode->children[0]->children[0], bones, Matrix4::identity(), animationTimeInTicks);
}


unsigned int Animation::findPosition(const Keyframe* keyframe) {
	assert(keyframe->positionKeys.size() > 0);
	for (unsigned int i = 0 ; i < keyframe->positionKeys.size() - 1 ; i++) {
		if (this->animationTimeInTicks < keyframe->positionKeys[i + 1].time) {
			return i;
		}
	}
	assert(0);
	return 0;
}


unsigned int Animation::findRotation(const Keyframe* keyframe) {
	assert(keyframe->rotationKeys.size() > 0);
	for (unsigned int i = 0 ; i < keyframe->rotationKeys.size() - 1 ; i++) {
		if (this->animationTimeInTicks < keyframe->rotationKeys[i + 1].time) {
			return i;
		}
	}
	assert(0);
	return 0;
}


unsigned int Animation::findScaling(const Keyframe* keyframe) {
	assert(keyframe->scalingKeys.size() > 0);
	for (unsigned int i = 0 ; i < keyframe->scalingKeys.size() - 1 ; i++) {
		if (this->animationTimeInTicks < keyframe->scalingKeys[i + 1].time) {
			return i;
		}
	}
	assert(0);
	return 0;
}


Vector3 Animation::calcInterpolatedPosition(const Keyframe* keyframe) {
	// need at least two values to interpolate
	if (keyframe->positionKeys.size() == 1) {
		return keyframe->positionKeys[0].value;
	}
	// find current keyframe index
	this->currentPositionIndex = findPosition(keyframe);
	unsigned int nextPositionIndex = (this->currentPositionIndex + 1);
	assert(nextPositionIndex < keyframe->positionKeys.size());
	// calculate blend factor
	float deltaTime = keyframe->positionKeys[nextPositionIndex].time - keyframe->positionKeys[this->currentPositionIndex].time;
	float factor = (this->animationTimeInTicks - keyframe->positionKeys[this->currentPositionIndex].time) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	// interpolate transformation
	Vector3 startValue = keyframe->positionKeys[this->currentPositionIndex].value;
	Vector3 endValue = keyframe->positionKeys[nextPositionIndex].value;
	Vector3 deltaValue = endValue - startValue;
	return startValue + factor * deltaValue;
}


Quaternion Animation::calcInterpolatedRotation(const Keyframe* keyframe) {
	// need at least two values to interpolate
	if (keyframe->rotationKeys.size() == 1) {
		return keyframe->rotationKeys[0].value;
	}
	// find current keyframe index
	this->currentRotationIndex = findRotation(keyframe);
	unsigned int nextRotationIndex = (this->currentRotationIndex + 1);
	assert(nextRotationIndex < keyframe->rotationKeys.size());
	// calculate blend factor
	float DeltaTime = keyframe->rotationKeys[nextRotationIndex].time - keyframe->rotationKeys[this->currentRotationIndex].time;
	float factor = (this->animationTimeInTicks - keyframe->rotationKeys[this->currentRotationIndex].time) / DeltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	// interpolate transformation
	aiQuaternion startValue = keyframe->rotationKeys[this->currentRotationIndex].value.getAissmp();
	aiQuaternion endValue   = keyframe->rotationKeys[nextRotationIndex].value.getAissmp();
	aiQuaternion interpolateValue;
	aiQuaternion::Interpolate(interpolateValue, startValue, endValue, factor);
	return interpolateValue.Normalize();
}


Vector3 Animation::calcInterpolatedScaling(const Keyframe* keyframe) {
	// need at least two values to interpolate
	if (keyframe->scalingKeys.size() == 1) {
		return keyframe->scalingKeys[0].value;
	}
	// find current keyframe index
	this->currentScalingIndex = findScaling(keyframe);
	unsigned int nextScalingIndex = (this->currentScalingIndex + 1);
	assert(nextScalingIndex < keyframe->scalingKeys.size());
	// calculate blend factor
	float deltaTime = keyframe->scalingKeys[nextScalingIndex].time - keyframe->scalingKeys[this->currentScalingIndex].time;
	float factor = (this->animationTimeInTicks - keyframe->scalingKeys[this->currentScalingIndex].time) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	// interpolate transformation
	Vector3 startValue = keyframe->scalingKeys[this->currentScalingIndex].value;
	Vector3 endValue   = keyframe->scalingKeys[nextScalingIndex].value;
	Vector3 deltaValue = endValue - startValue;
	return startValue + factor * deltaValue;
}
