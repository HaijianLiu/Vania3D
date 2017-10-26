
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< update pose >
update pose data
via animations keyframes data and the given time in seconds
------------------------------------------------------------------------------*/
void Model::updatePose(unsigned int animationIndex, float time) {
	float timeInTicks = time * this->animations[animationIndex]->ticksPerSecond;
	float animationTimeInTicks = fmod(timeInTicks, this->animations[animationIndex]->duration);
	this->processPose(this->animations[animationIndex], animationTimeInTicks, this->rootNode, Matrix4::identity());
}


void Model::processPose(const Animation* animation, float animationTimeInTicks, const Node<Matrix4>* node, const Matrix4& parentTransformation) {

	Matrix4 nodeTransformation = node->data;

	const Keyframe* keyframe = FindNodeAnim(animation, node->name);

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
		nodeTransformation = TranslationM * RotationM * ScalingM;
	}

	Matrix4 globalTransformation = parentTransformation * nodeTransformation;


	if (this->bones.find(node->name) != this->bones.end()) {
		unsigned int boneIndex = this->bones[node->name].index;
		this->pose[boneIndex] = this->rootNode->data * globalTransformation * this->bones[node->name].offset;
	}

	for (unsigned int i = 0 ; i < node->children.size() ; i++) {
		this->processPose(animation, animationTimeInTicks, node->children[i], globalTransformation);
	}
}


const Keyframe* Model::FindNodeAnim(const Animation* animation, const std::string nodeName)
{
    for (unsigned int i = 0 ; i < animation->keyframes.size() ; i++) {
        const Keyframe* keyframe = animation->keyframes[i];

			if (keyframe->nodeName == nodeName) {
            return keyframe;
        }
    }

    return nullptr;
}


uint Model::FindPosition(float animationTimeInTicks, const Keyframe* keyframe)
{
    for (unsigned int i = 0 ; i < keyframe->positionKeys.size() - 1 ; i++) {
        if (animationTimeInTicks < keyframe->positionKeys[i + 1].time) {
            return i;
        }
    }

    assert(0);

    return 0;
}


uint Model::FindRotation(float animationTimeInTicks, const Keyframe* keyframe)
{
    assert(keyframe->rotationKeys.size() > 0);

    for (unsigned int i = 0 ; i < keyframe->rotationKeys.size() - 1 ; i++) {
        if (animationTimeInTicks < keyframe->rotationKeys[i + 1].time) {
            return i;
        }
    }

    assert(0);

    return 0;
}


uint Model::FindScaling(float animationTimeInTicks, const Keyframe* keyframe)
{
    assert(keyframe->scalingKeys.size() > 0);

    for (unsigned int i = 0 ; i < keyframe->scalingKeys.size() - 1 ; i++) {
        if (animationTimeInTicks < keyframe->scalingKeys[i + 1].time) {
            return i;
        }
    }

    assert(0);

    return 0;
}


void Model::CalcInterpolatedPosition(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe)
{
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


void Model::CalcInterpolatedRotation(Quaternion& Out, float animationTimeInTicks, const Keyframe* keyframe)
{
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


void Model::CalcInterpolatedScaling(Vector3& Out, float animationTimeInTicks, const Keyframe* keyframe)
{
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

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Model::Model(const char* path) {
	this->rootNode = new Node<Matrix4>("root");
	this->load(path);
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Model::~Model() {
	delete this->rootNode;
	deleteVector(this->meshes);
	deleteVector(this->animations);
}


/*------------------------------------------------------------------------------
< draw >
------------------------------------------------------------------------------*/
void Model::draw() {
	for(unsigned int i = 0; i < this->meshes.size(); i++) this->meshes[i]->draw();
}


/*------------------------------------------------------------------------------
< load model >
------------------------------------------------------------------------------*/
// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void Model::load(const char* path) {
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* aiscene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if(!aiscene || aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiscene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	this->rootNode->data = aiscene->mRootNode->mTransformation;
	// process assimp root node recursively
	this->processNode(aiscene->mRootNode, this->rootNode, aiscene);
	// copy all assimp animation data
	this->processAnimation(aiscene);

	// for test
	this->updatePose(0, 1.0);
}


/*------------------------------------------------------------------------------
< process node >
processes a node in a recursive function
the node object only contains indices to index the actual objects in the scene
the scene contains all the data
Processes each individual mesh located at the node and repeats this process on its children nodes (if any)
Processes the bone node heirarchy located at the node and calculate the final transformation
------------------------------------------------------------------------------*/
void Model::processNode(aiNode* ainode, Node<Matrix4>* node, const aiScene* aiscene) {
	// process each mesh located at the current node
	for (unsigned int i = 0; i < ainode->mNumMeshes; i++) {
		aiMesh* mesh = aiscene->mMeshes[ainode->mMeshes[i]];
		this->meshes.push_back(createMesh(mesh, aiscene));
	}
	// save the node heirarchy and all the transformation matrices and names
	node->data = ainode->mTransformation;

	// recursively process each of the children nodes
	for (unsigned int i = 0; i < ainode->mNumChildren; i++) {
		node->children.push_back(new Node<Matrix4>(ainode->mChildren[i]->mName.data));
		node->children[i]->parent = node;
		this->processNode(ainode->mChildren[i], node->children[i], aiscene);
	}
}


/*------------------------------------------------------------------------------
< process Animation >
processes each individual aiAnimation located at the aiScene
processes each individual channel (aiNodeAnim) located at the aiAnimation
copy all the keyframe data to the animations member
------------------------------------------------------------------------------*/
void Model::processAnimation(const aiScene* aiscene) {
	// copy aiAnimation
	for (unsigned int i = 0; i < aiscene->mNumAnimations; i++) {
		this->animations.push_back(new Animation());
		this->animations[i]->name = aiscene->mAnimations[i]->mName.data;
		this->animations[i]->duration = aiscene->mAnimations[i]->mDuration;
		this->animations[i]->ticksPerSecond = aiscene->mAnimations[i]->mTicksPerSecond;
		// copy aiNodeAnim
		for (unsigned int j = 0; j < aiscene->mAnimations[i]->mNumChannels; j++) {
			this->animations[i]->keyframes.push_back(new Keyframe());
			this->animations[i]->keyframes[j]->nodeName = aiscene->mAnimations[i]->mChannels[j]->mNodeName.data;
			// copy keyframes
			for (unsigned int k = 0; k < aiscene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++) {
				VectorKey vectorKey;
				vectorKey.time = aiscene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;
				vectorKey.value = aiscene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue;
				this->animations[i]->keyframes[j]->positionKeys.push_back(vectorKey);
			}
			for (unsigned int k = 0; k < aiscene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++) {
				QuaternionKey quaternionKey;
				quaternionKey.time = aiscene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
				quaternionKey.value = aiscene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue;
				this->animations[i]->keyframes[j]->rotationKeys.push_back(quaternionKey);
			}
			for (unsigned int k = 0; k < aiscene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++) {
				VectorKey vectorKey;
				vectorKey.time = aiscene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime;
				vectorKey.value = aiscene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue;
				this->animations[i]->keyframes[j]->scalingKeys.push_back(vectorKey);
			}
		}
	}
}


/*------------------------------------------------------------------------------
< create mesh>
load mesh vertices data to vao
if vertices have bone weights then load them to vao too
------------------------------------------------------------------------------*/
Mesh* Model::createMesh(aiMesh* mesh, const aiScene* scene) {
	// data to fill
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	/* vertices */
	// walk through each of the mesh's vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// texture coordinates
		// does the mesh contain texture coordinates?
		if(mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates
			// use models where a vertex can have multiple texture coordinates
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;
		}
		else vertex.uv = glm::vec2(0.0f, 0.0f);
		// tangent
		// vector.x = mesh->mTangents[i].x;
		// vector.y = mesh->mTangents[i].y;
		// vector.z = mesh->mTangents[i].z;
		// vertex.tangent = vector;
		// bitangent
		// vector.x = mesh->mBitangents[i].x;
		// vector.y = mesh->mBitangents[i].y;
		// vector.z = mesh->mBitangents[i].z;
		// vertex.bitangent = vector;
		vertices.push_back(vertex);
	}

	/* indices */
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	/* bones */
	unsigned int counter = 0;
	for (unsigned int i = 0 ; i < mesh->mNumBones ; i++) {
		unsigned int boneIndex = 0;
		std::string boneName = mesh->mBones[i]->mName.data;

		// calculate bone index
		// get bone offset from mesh
		// create bones and a default pose
		if (this->bones.find(boneName) == this->bones.end()) {
			boneIndex = counter;
			counter++;
			Bone bone = Bone(boneIndex, mesh->mBones[i]->mOffsetMatrix);
			this->bones[boneName] = bone;
			this->pose.push_back(Matrix4::identity());
		}
		else {
			boneIndex = this->bones[boneName].index;
		}

		// set vertices
		for (unsigned int j = 0 ; j < mesh->mBones[i]->mNumWeights ; j++) {
			unsigned int vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;

			for (int k = 0 ; k < NUM_BONES_PER_VEREX ; k++) {
				if (vertices[vertexID].weight[k] == 0.0) {
					vertices[vertexID].boneID[k] = boneIndex;
					vertices[vertexID].weight[k] = weight;
					break;
				}
				if (k == NUM_BONES_PER_VEREX - 1) {
					printf("[WARNNING] vertex: %d weight: %f out of the number of NUM_BONES_PER_VEREX!\n", vertexID, weight);
				}
			}

		}
	}

	// return a mesh object created from the extracted mesh data
	return new Mesh(vertices, indices);
}
