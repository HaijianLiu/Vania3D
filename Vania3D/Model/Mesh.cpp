
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int attributeType) {
	this->attributeType = attributeType;
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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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
	}
	
	glBindVertexArray(0);
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

void Mesh::drawBounding() {
	glBindVertexArray(this->vaoBounding);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
