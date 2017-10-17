
#ifndef Mesh_hpp
#define Mesh_hpp

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
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
