
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));
	// vertex uv
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));
	// vertex bitangent
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, NUM_BONES_PER_VEREX, GL_UNSIGNED_INT, sizeof(Vertex), (void*)offsetof(Vertex,boneID));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, NUM_BONES_PER_VEREX, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,weight));

	glBindVertexArray(0);
	// glDeleteBuffers(1, &vbo);
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
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, this->count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::drawBounding() {
	glBindVertexArray(this->vaoBounding);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
