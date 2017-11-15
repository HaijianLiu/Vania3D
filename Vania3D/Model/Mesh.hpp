
#ifndef Mesh_hpp
#define Mesh_hpp

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	unsigned int boneID[NUM_BONES_PER_VEREX] = {NULL};
	float weight[NUM_BONES_PER_VEREX] = {0.0};
};

class Mesh {
	friend class Model;
private:
	unsigned int vao, count, vaoBounding;

public:
	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();

	void draw();
	void drawBounding();
};

#endif /* Mesh_hpp */
