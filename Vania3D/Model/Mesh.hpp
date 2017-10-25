
#ifndef Mesh_hpp
#define Mesh_hpp

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	unsigned int boneID[NUM_BONES_PER_VEREX] = {NULL};
	float weight[NUM_BONES_PER_VEREX] = {0.0};
};

class Mesh {
private:
	unsigned int vao, count;

public:
	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();

	/* functions */
	void draw();
};

#endif /* Mesh_hpp */
