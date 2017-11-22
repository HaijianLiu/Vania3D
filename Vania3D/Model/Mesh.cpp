
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Mesh::Mesh(const aiMesh* aimesh, std::vector<glm::mat4>* pose, unsigned int attributeType) {
	this->attributeType = attributeType;
	
	switch (attributeType) {
		case MESH_ATTRIBUTE_DEFAULT:
			this->createDefaultMesh(aimesh);
			break;
		case MESH_ATTRIBUTE_INSTANCE:
			this->createInstanceMesh(aimesh);
			break;
		case MESH_ATTRIBUTE_BONE:
			this->createBoneMesh(aimesh);
			break;
		case MESH_ATTRIBUTE_INSTANCE_FX:
			this->createFxMesh(aimesh);
			break;
	}
	
	// data to fill
	std::vector<Vertex> vertices;
	// vertices
	for(unsigned int i = 0; i < aimesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position = assignment(aimesh->mVertices[i]);
		vertex.normal = assignment(aimesh->mNormals[i]);
		if(aimesh->mTextureCoords[0]) vertex.uv = assignment(aimesh->mTextureCoords[0][i]);
		else vertex.uv = glm::vec2(0.0f, 0.0f);
		this->updateBounding(vertex.position, this->boundingMax, this->boundingMin);
		vertices.push_back(vertex);
	}
	// bones
	this->boneMapping(&vertices, pose, aimesh);
	// indices
	std::vector<unsigned int> indices;
	this->loadIndices(&indices, aimesh);
	
	
	
	// create bounding box
	this->vaoBounding = this->createBox(this->boundingMax, this->boundingMin);
	
	

	
	
	// vertex buffer, element buffer
	unsigned int vbo, ebo;
	// set the vertex buffers and its attribute pointers.
	// create buffers/arrays
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// bind
	glBindVertexArray(this->vao);
	this->count = indices.size();

	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	// vertex uv
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// switch attribute type
	switch (attributeType) {
		case MESH_ATTRIBUTE_BONE:
			// vertex bone id
			glEnableVertexAttribArray(3);
			glVertexAttribIPointer(3, NUM_BONES_PER_VEREX, GL_UNSIGNED_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneID));
			// vertex bone weight
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, NUM_BONES_PER_VEREX, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weight));
			break;

		case MESH_ATTRIBUTE_INSTANCE:
			glGenBuffers(1, &this->vboInstanceMatrix);
			glBindBuffer(GL_ARRAY_BUFFER, this->vboInstanceMatrix);
			glBufferData(GL_ARRAY_BUFFER, MAX_MESH_INSTANCE * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)sizeof(glm::vec4));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			break;
		case MESH_ATTRIBUTE_INSTANCE_FX:
			glGenBuffers(1, &this->vboInstanceMatrix);
			glBindBuffer(GL_ARRAY_BUFFER, this->vboInstanceMatrix);
			glBufferData(GL_ARRAY_BUFFER, MAX_MESH_INSTANCE * sizeof(InstanceFx), nullptr, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceFx), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceFx), (void*)sizeof(glm::vec4));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceFx), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceFx), (void*)(3 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceFx), (void*)(4 * sizeof(glm::vec4)));
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);
			break;
	}

	glBindVertexArray(0);
}


void Mesh::createDefaultMesh(const aiMesh* aimesh) {
	
}
void Mesh::createInstanceMesh(const aiMesh* aimesh) {
	
}

void Mesh::createBoneMesh(const aiMesh* aimesh) {
	
}
void Mesh::createFxMesh(const aiMesh* aimesh) {
	
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Mesh::~Mesh() {
	glDeleteVertexArrays(1, &this->vao);
}



/*------------------------------------------------------------------------------
< draw >
------------------------------------------------------------------------------*/
void Mesh::draw() {
	glDrawElements(GL_TRIANGLES, this->count, GL_UNSIGNED_INT, 0);
}

void Mesh::drawInstance(std::vector<glm::mat4>* instanceMatrices) {
	glBindBuffer(GL_ARRAY_BUFFER, this->vboInstanceMatrix);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instanceMatrices->size() * sizeof(glm::mat4), instanceMatrices->data());
	glDrawElementsInstanced(GL_TRIANGLES, this->count, GL_UNSIGNED_INT, 0, instanceMatrices->size());
}

void Mesh::drawFX(std::vector<InstanceFx>* instances) {
	glBindBuffer(GL_ARRAY_BUFFER, this->vboInstanceMatrix);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instances->size() * sizeof(InstanceFx), instances->data());
	glDrawElementsInstanced(GL_TRIANGLES, this->count, GL_UNSIGNED_INT, 0, instances->size());
}

void Mesh::drawBounding() {
	glBindVertexArray(this->vaoBounding);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


/*------------------------------------------------------------------------------
 < create bounding box >
 ------------------------------------------------------------------------------*/
unsigned int Mesh::createBox(glm::vec3 boundingMax, glm::vec3 boundingMin) {
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

void Mesh::loadIndices(std::vector<unsigned int>* indices, const aiMesh* aimesh) {
	for(unsigned int i = 0; i < aimesh->mNumFaces; i++) {
		aiFace face = aimesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices->push_back(face.mIndices[j]);
	}
}


void Mesh::boneMapping(std::vector<Vertex>* vertices, std::vector<glm::mat4>* pose, const aiMesh* aimesh) {
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


void Mesh::updateBounding(glm::vec3 vertexPosition, glm::vec3& boundingMax, glm::vec3& boundingMin) {
	if (vertexPosition.x > boundingMax.x) boundingMax.x = vertexPosition.x;
	if (vertexPosition.x < boundingMin.x) boundingMin.x = vertexPosition.x;
	if (vertexPosition.y > boundingMax.y) boundingMax.y = vertexPosition.y;
	if (vertexPosition.y < boundingMin.y) boundingMin.y = vertexPosition.y;
	if (vertexPosition.z > boundingMax.z) boundingMax.z = vertexPosition.z;
	if (vertexPosition.z < boundingMin.z) boundingMin.z = vertexPosition.z;
}

