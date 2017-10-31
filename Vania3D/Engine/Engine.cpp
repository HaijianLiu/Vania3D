
#define STB_IMAGE_IMPLEMENTATION

#include "Engine.hpp"

/*------------------------------------------------------------------------------
< convert from Assimp to glm >
------------------------------------------------------------------------------*/
glm::vec3 assignment(const aiVector3D& assimpVector) {
	return glm::vec3(assimpVector.x, assimpVector.y, assimpVector.z);
}

glm::quat assignment(const aiQuaternion& assimpQuaternion) {
	return glm::quat(assimpQuaternion.w, assimpQuaternion.x, assimpQuaternion.y, assimpQuaternion.z);
}

glm::mat4 assignment(const aiMatrix4x4& assimpMatrix) {
	glm::mat4 result = {
		assimpMatrix.a1, assimpMatrix.b1, assimpMatrix.c1, assimpMatrix.d1,
		assimpMatrix.a2, assimpMatrix.b2, assimpMatrix.c2, assimpMatrix.d2,
		assimpMatrix.a3, assimpMatrix.b3, assimpMatrix.c3, assimpMatrix.d3,
		assimpMatrix.a4, assimpMatrix.b4, assimpMatrix.c4, assimpMatrix.d4
	};
	return result;
}

glm::mat4 assignment(const aiMatrix3x3& assimpMatrix) {
	glm::mat4 result = {
		assimpMatrix.a1, assimpMatrix.b1, assimpMatrix.c1, 0.0,
		assimpMatrix.a2, assimpMatrix.b2, assimpMatrix.c2, 0.0,
		assimpMatrix.a3, assimpMatrix.b3, assimpMatrix.c3, 0.0,
		            0.0,             0.0,             0.0, 1.0
	};
	return result;
}


/*------------------------------------------------------------------------------
< math >
------------------------------------------------------------------------------*/
// Returns a quaternion such that q*start = dest
glm::quat rotationBetweenVectors(glm::vec3 start, glm::vec3 end){
	start = glm::normalize(start);
	end = glm::normalize(end);

	float cosTheta = glm::dot(start, end);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){
		// special case when vectors in opposite directions :
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		// This implementation favors a rotation around the Up axis,
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0f), rotationAxis);
	}

	// Implementation from Stan Melax's Game Programming Gems 1 article
	rotationAxis = cross(start, end);

	float s = glm::sqrt( (1+cosTheta)*2 );
	float invs = 1 / s;

	return glm::quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);
}
