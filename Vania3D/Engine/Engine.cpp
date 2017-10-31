
#define STB_IMAGE_IMPLEMENTATION

#include "Engine.hpp"

glm::vec3 assignment(const aiVector3D& assimpVector) {
	return glm::vec3(assimpVector.x, assimpVector.y, assimpVector.z);
}
