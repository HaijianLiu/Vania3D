
#ifndef Mesh_hpp
#define Mesh_hpp

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	unsigned int boneID[NUM_BONES_PER_VEREX] = {NULL};
	float weight[NUM_BONES_PER_VEREX] = {0.0};
};

//struct VertexBone {
//	glm::vec3 position;
//	glm::vec3 normal;
//	glm::vec2 uv;
//	unsigned int boneID[NUM_BONES_PER_VEREX] = {NULL};
//	float weight[NUM_BONES_PER_VEREX] = {0.0};
//};
//
//struct VertexFx {
//	glm::vec3 position;
//	glm::vec3 normal;
//	glm::vec2 uv;
//};

struct InstanceFx {
	glm::mat4 model;
	float animationTime;
};

class Mesh {
	friend class Model;
	friend class FrustumCulling;
	friend class MaterialLayer;
	friend class MeshRenderer;
	friend class MaterialLayer;

private:
	unsigned int vao, count, vaoBounding, vboInstanceMatrix, instanceSize = 0;
	glm::vec3 boundingMax, boundingMin;
	unsigned int attributeType;

public:
	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int attributeType);
	~Mesh();

	void draw();
	void drawInstance(std::vector<glm::mat4>* instanceMatrices);
	void drawFX(std::vector<InstanceFx>* instances);
	void drawBounding();
};

#endif /* Mesh_hpp */
