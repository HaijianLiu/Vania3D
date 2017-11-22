
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

private:
	unsigned int vao, count, vaoBounding, vboInstanceMatrix, instanceSize = 0;
	glm::vec3 boundingMax = glm::vec3(0), boundingMin = glm::vec3(0);
	unsigned int attributeType;
	
	void createDefaultMesh(const aiMesh* aimesh);
	void createInstanceMesh(const aiMesh* aimesh);
	void createBoneMesh(const aiMesh* aimesh);
	void createFxMesh(const aiMesh* aimesh);
	
	static void loadIndices(std::vector<unsigned int>* indices, const aiMesh* aimesh);
	static void boneMapping(std::vector<Vertex>* vertices, std::vector<glm::mat4>* pose, const aiMesh* aimesh);
	static void updateBounding(glm::vec3 vertexPosition, glm::vec3& boundingMax, glm::vec3& boundingMin);
	static unsigned int createBox(glm::vec3 boundingMax, glm::vec3 boundingMin);

public:
	// constructor
	Mesh(const aiMesh* aimesh, std::vector<glm::mat4>* pose, unsigned int attributeType);
	~Mesh();

	void draw();
	void drawInstance(std::vector<glm::mat4>* instanceMatrices);
	void drawFX(std::vector<InstanceFx>* instances);
	void drawBounding();
};

#endif /* Mesh_hpp */
