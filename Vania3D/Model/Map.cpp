
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Map::Map(const char* path) {
	this->load(path);
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Map::~Map() {
	delete this->modelNode;
}


/*------------------------------------------------------------------------------
< load model >
loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
------------------------------------------------------------------------------*/
void Map::load(const char* path) {
	Game* game = Game::getInstance();
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* aiscene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights);
	// check for errors
	if(!aiscene || aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiscene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	// mapping materials
	for (unsigned int i = 0; i < aiscene->mNumMaterials; i++) {
		std::string materialName(aiscene->mMaterials[i]->mProperties[0]->mData + 4 ,aiscene->mMaterials[i]->mProperties[0]->mDataLength - 5); // magic numbers!
		Material* material = game->resources->getMaterial(materialName);
		this->materialMapping.push_back(material); // if no material also push a nullptr
	}

	// process assimp root node recursively
	this->modelNode = new Node<glm::mat4>(aiscene->mRootNode->mName.data);
	this->modelNode->data = new glm::mat4();
	this->processNode(aiscene->mRootNode, this->modelNode, aiscene);
}


/*------------------------------------------------------------------------------
< process node >
processes a node in a recursive function
the node object only contains indices to index the actual objects in the scene
the scene contains all the data
Processes each individual mesh located at the node and repeats this process on its children nodes (if any)
Processes the bone node heirarchy located at the node and calculate the final transformation
------------------------------------------------------------------------------*/
void Map::processNode(aiNode* ainode, Node<glm::mat4>* node, const aiScene* aiscene) {
	// save the node heirarchy and all the transformation matrices and names
	*node->data = assignment(ainode->mTransformation);

	/* look for children */
	// recursively process each of the children nodes
	for (unsigned int i = 0; i < ainode->mNumChildren; i++) {
		node->children.push_back(new Node<glm::mat4>(ainode->mChildren[i]->mName.data));
		node->children[i]->parent = node;
		node->children[i]->data = new glm::mat4();
		this->processNode(ainode->mChildren[i], node->children[i], aiscene);
	}
}


void Map::createGameObjects(Scene* scene) {
	Game* game = Game::getInstance();
	glm::mat4 parentTransformation = glm::scale(glm::vec3(0.05));
	this->processNode(this->modelNode, parentTransformation, game, scene);
}

void Map::processNode(Node<glm::mat4>* node, glm::mat4 parentTransformation, Game* game, Scene* scene) {
	// create GameObject
	glm::mat4 globalTransformation = parentTransformation * (*node->data);
	std::string modelName = node->name.substr(0,node->name.rfind("."));
	Model* model = game->resources->getModel(modelName);
	if (model != nullptr) {
		GameObject* gameObject = new GameObject();
		Transform* transform = gameObject->addComponent<Transform>();
		transform->model = globalTransformation;
		transform->init = false;
		MeshRenderer* meshRenderer = gameObject->addComponent<MeshRenderer>();
		meshRenderer->model = model;
		// tes
		for (unsigned int i = 0; i < 3; i++) {
			meshRenderer->materials.push_back(this->materialMapping[i]);
		}
		meshRenderer->lightProbe = game->resources->getLightProbe("hdr");
		scene->addGameObject(node->name.c_str(), gameObject);
	}

	for (unsigned int i = 0; i < node->children.size(); i++)
		this->processNode(node->children[i], globalTransformation, game, scene);
}
