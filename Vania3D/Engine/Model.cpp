
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Model::Model(const char* path) {
	this->load(path);
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Model::~Model() {

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
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// process ASSIMP's root node recursively
	Model::processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode* node, const aiScene* scene) {
	// process each mesh located at the current node
	for(unsigned int i = 0; i < node->mNumMeshes; i++) {
		// the node object only contains indices to index the actual objects in the scene.
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for(unsigned int i = 0; i < node->mNumChildren; i++) {
		Model::processNode(node->mChildren[i], scene);
	}
}

struct BoneInfo {
	aiMatrix4x4 offset;
};


Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	// data to fill
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	/* vertices */
	// Walk through each of the mesh's vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
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
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;
		}
		else vertex.uv = glm::vec2(0.0f, 0.0f);
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.bitangent = vector;
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
	std::map<const char*, unsigned int> boneMapping;
	std::vector<BoneInfo> boneInfo;
	unsigned int m_NumBones = 0;

	// set bones
	for (unsigned int i = 0 ; i < mesh->mNumBones ; i++) {
		unsigned int boneIndex = 0;
		const char* boneName = mesh->mBones[i]->mName.data;

		// index
		if (boneMapping.find(boneName) == boneMapping.end()) {
			boneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			boneInfo.push_back(bi);
		}
		else {
			boneIndex = boneMapping[boneName];
		}
		boneMapping[boneName] = boneIndex;
		boneInfo[boneIndex].offset = mesh->mBones[i]->mOffsetMatrix;

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
					printf("[WARNNING]vertex: %d weight: %f out of the number of NUM_BONES_PER_VEREX!\n", vertexID, weight);
				}
			}

		}
	}

	// return a mesh object created from the extracted mesh data
	return new Mesh(vertices, indices);
}
