
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Model::Model() {

}

Model::Model(unsigned int attributeType, const char* path) {
	this->attributeType = attributeType;
	this->load(path);
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Model::~Model() {
	delete this->boneNode;
	deleteVector(this->meshes);
	deleteVector(this->animations);
}


/*------------------------------------------------------------------------------
< update pose >
update pose data
via animations keyframes data and the given time in seconds
------------------------------------------------------------------------------*/
void Model::updatePose(unsigned int animationIndex, float timeInSeconds) {
	// animation always starts from the beginning
	if (this->currentAnimation != animationIndex) {
		this->animations[animationIndex]->reset(timeInSeconds);
		this->currentAnimation = animationIndex;
	}

	this->animations[animationIndex]->updatePose(this->pose, this->boneNode, timeInSeconds);
}


/*------------------------------------------------------------------------------
< load animation >
loads a model with supported ASSIMP extensions from file and stores the resulting animation keyframes in a node tree.
------------------------------------------------------------------------------*/
void Model::addAnimation(Animation* animation) {
	this->animations.push_back(animation);
}


/*------------------------------------------------------------------------------
< draw >
------------------------------------------------------------------------------*/
void Model::draw() {
	for(unsigned int i = 0; i < this->meshes.size(); i++) this->meshes[i]->draw();
}

void Model::drawMesh(unsigned int meshIndex) {
	if (meshIndex < this->meshes.size())
		this->meshes.at(meshIndex)->draw();
}

void Model::drawBounding() {
	for(unsigned int i = 0; i < this->meshes.size(); i++) this->meshes[i]->drawBounding();
}


/*------------------------------------------------------------------------------
< set position uniform >
------------------------------------------------------------------------------*/
void Model::setPoseUniform(Shader* shader) {
	for (unsigned int i = 0 ; i < this->pose.size() ; i++) {
		std::string boneName = UNIFORM_MATRIX_BONE;
		shader->setMat4((boneName + "[" + std::to_string(i) + "]").c_str(), this->pose[i]);
	}
}


/*------------------------------------------------------------------------------
< load model >
loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
------------------------------------------------------------------------------*/
void Model::load(const char* path) {
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* aiscene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights);
	// check for errors
	if(!aiscene || aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiscene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	// process assimp root node recursively
	this->boneNode = new Node<Bone>(aiscene->mRootNode->mName.data);
	this->boneNode->data = new Bone();
	this->boneNode->data->nodeTransformation = assignment(aiscene->mRootNode->mTransformation);
	this->processNode(aiscene->mRootNode, this->boneNode, aiscene);
}


/*------------------------------------------------------------------------------
< process node >
processes a node in a recursive function
the node object only contains indices to index the actual objects in the scene
the scene contains all the data
Processes each individual mesh located at the node and repeats this process on its children nodes (if any)
Processes the bone node heirarchy located at the node and calculate the final transformation
------------------------------------------------------------------------------*/
void Model::processNode(aiNode* ainode, Node<Bone>* node, const aiScene* aiscene) {
	/* if found a mesh */
	// process each mesh located at the current node
	for (unsigned int i = 0; i < ainode->mNumMeshes; i++) {
		aiMesh* mesh = aiscene->mMeshes[ainode->mMeshes[i]];
		this->createMesh(mesh, aiscene);
	}
	// save the node heirarchy and all the transformation matrices and names
	node->data->nodeTransformation = assignment(ainode->mTransformation);
	// only support one mesh model !
	for (unsigned int i = 0; i < aiscene->mMeshes[0]->mNumBones; i ++) {
		if (node->name == aiscene->mMeshes[0]->mBones[i]->mName.data) {
			node->data->haveBone = true;
			node->data->index = i;
			node->data->offset = assignment(aiscene->mMeshes[0]->mBones[i]->mOffsetMatrix);
			break;
		}
	}

	/* look for children */
	// recursively process each of the children nodes
	for (unsigned int i = 0; i < ainode->mNumChildren; i++) {
		node->children.push_back(new Node<Bone>(ainode->mChildren[i]->mName.data));
		node->children[i]->parent = node;
		node->children[i]->data = new Bone();
		this->processNode(ainode->mChildren[i], node->children[i], aiscene);
	}
}


/*------------------------------------------------------------------------------
< create mesh>
load mesh vertices data to vao
if vertices have bone weights then load them to vao too
------------------------------------------------------------------------------*/
void Model::createMesh(aiMesh* mesh, const aiScene* scene) {
	
	// data to fill
	std::vector<Vertex> vertices;
	
	// for generating bounding box
	glm::vec3 boundingMax = glm::vec3(0);
	glm::vec3 boundingMin = glm::vec3(0);

	// vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position = assignment(mesh->mVertices[i]);
		vertex.normal = assignment(mesh->mNormals[i]);
		if(mesh->mTextureCoords[0]) vertex.uv = assignment(mesh->mTextureCoords[0][i]);
		else vertex.uv = glm::vec2(0.0f, 0.0f);
		this->updateBounding(vertex.position, boundingMax, boundingMin);
		vertices.push_back(vertex);
	}

	// bones
	this->boneMapping(&vertices, &this->pose, mesh);
	
	
	// indices
	std::vector<unsigned int> indices;
	for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	
	// return a mesh object created from the extracted mesh data
	this->meshes.push_back(new Mesh(vertices, indices, this->attributeType));
	// create bounding box
	this->meshes.back()->vaoBounding = this->createBox(boundingMax, boundingMin);
	this->meshes.back()->boundingMax = boundingMax;
	this->meshes.back()->boundingMin = boundingMin;
}



void Model::boneMapping(std::vector<Vertex>* vertices, std::vector<glm::mat4>* pose, const aiMesh* aimesh) {
	std::unordered_map<std::string, Bone> boneMapping;
	unsigned int counter = 0;
	for (unsigned int i = 0 ; i < aimesh->mNumBones ; i++) {
		unsigned int boneIndex = 0;
		std::string boneName = aimesh->mBones[i]->mName.data;
		
		// calculate bone index, get bone offset from mesh, create bones and a default pose
		if (boneMapping.find(boneName) == boneMapping.end()) {
			boneIndex = counter;
			counter++;
			glm::mat4 offset = assignment(aimesh->mBones[i]->mOffsetMatrix);
			Bone bone = Bone(boneIndex, offset);
			boneMapping[boneName] = bone;
			pose->push_back(glm::mat4(1.0));
		}
		else {
			boneIndex = boneMapping[boneName].index;
		}
		
		// set vertices
		for (unsigned int j = 0 ; j < aimesh->mBones[i]->mNumWeights ; j++) {
			unsigned int vertexID = aimesh->mBones[i]->mWeights[j].mVertexId;
			float weight = aimesh->mBones[i]->mWeights[j].mWeight;
			
			for (int k = 0 ; k < NUM_BONES_PER_VEREX ; k++) {
				if (vertices->at(vertexID).weight[k] == 0.0) {
					vertices->at(vertexID).boneID[k] = boneIndex;
					vertices->at(vertexID).weight[k] = weight;
					break;
				}
				if (k == NUM_BONES_PER_VEREX - 1) {
					printf("[WARNNING] vertex: %d weight: %f out of the number of NUM_BONES_PER_VEREX!\n", vertexID, weight);
				}
			}
		}
	}
}


void Model::updateBounding(glm::vec3 vertexPosition, glm::vec3& boundingMax, glm::vec3& boundingMin) {
	if (vertexPosition.x > boundingMax.x) boundingMax.x = vertexPosition.x;
	if (vertexPosition.x < boundingMin.x) boundingMin.x = vertexPosition.x;
	if (vertexPosition.y > boundingMax.y) boundingMax.y = vertexPosition.y;
	if (vertexPosition.y < boundingMin.y) boundingMin.y = vertexPosition.y;
	if (vertexPosition.z > boundingMax.z) boundingMax.z = vertexPosition.z;
	if (vertexPosition.z < boundingMin.z) boundingMin.z = vertexPosition.z;
}


/*------------------------------------------------------------------------------
< create box via six values >
------------------------------------------------------------------------------*/
unsigned int Model::createBox(glm::vec3 boundingMax, glm::vec3 boundingMin) {
	float boxVertices[] = {
		boundingMin.x, boundingMax.y, boundingMin.z,
		boundingMin.x, boundingMin.y, boundingMin.z,
		boundingMax.x, boundingMin.y, boundingMin.z,
		boundingMax.x, boundingMin.y, boundingMin.z,
		boundingMax.x, boundingMax.y, boundingMin.z,
		boundingMin.x, boundingMax.y, boundingMin.z,

		boundingMin.x, boundingMin.y, boundingMax.z,
		boundingMin.x, boundingMin.y, boundingMin.z,
		boundingMin.x, boundingMax.y, boundingMin.z,
		boundingMin.x, boundingMax.y, boundingMin.z,
		boundingMin.x, boundingMax.y, boundingMax.z,
		boundingMin.x, boundingMin.y, boundingMax.z,

		boundingMax.x, boundingMin.y, boundingMin.z,
		boundingMax.x, boundingMin.y, boundingMax.z,
		boundingMax.x, boundingMax.y, boundingMax.z,
		boundingMax.x, boundingMax.y, boundingMax.z,
		boundingMax.x, boundingMax.y, boundingMin.z,
		boundingMax.x, boundingMin.y, boundingMin.z,

		boundingMin.x, boundingMin.y, boundingMax.z,
		boundingMin.x, boundingMax.y, boundingMax.z,
		boundingMax.x, boundingMax.y, boundingMax.z,
		boundingMax.x, boundingMax.y, boundingMax.z,
		boundingMax.x, boundingMin.y, boundingMax.z,
		boundingMin.x, boundingMin.y, boundingMax.z,

		boundingMin.x, boundingMax.y, boundingMin.z,
		boundingMax.x, boundingMax.y, boundingMin.z,
		boundingMax.x, boundingMax.y, boundingMax.z,
		boundingMax.x, boundingMax.y, boundingMax.z,
		boundingMin.x, boundingMax.y, boundingMax.z,
		boundingMin.x, boundingMax.y, boundingMin.z,

		boundingMin.x, boundingMin.y, boundingMin.z,
		boundingMin.x, boundingMin.y, boundingMax.z,
		boundingMax.x, boundingMin.y, boundingMin.z,
		boundingMax.x, boundingMin.y, boundingMin.z,
		boundingMin.x, boundingMin.y, boundingMax.z,
		boundingMax.x, boundingMin.y, boundingMax.z
	};
	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), &boxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	return vao;
}
