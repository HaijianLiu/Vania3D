
#include "Engine.hpp"

FrustumCulling::FrustumCulling() {

}

FrustumCulling::~FrustumCulling() {

}

void FrustumCulling::start() {
	this->transform = this->gameObject->getComponent<Transform>();
	this->camera = this->gameObject->getComponent<Camera>();

	// compute width and height of the near and far plane sections
	float ratioTangent = glm::tan(glm::radians(this->camera->field * 0.5));
	this->nearHeight = this->camera->rangeStart * ratioTangent;
	this->nearWidth = nearHeight * this->camera->ratio;
	this->farHeight = this->camera->rangeEnd * ratioTangent;
	this->farWidth = farHeight * this->camera->ratio;
}


void FrustumCulling::update() {
	glm::vec3 nearCenter = this->transform->position + this->camera->front * this->camera->rangeStart;
	glm::vec3 farCenter = this->transform->position + this->camera->front * this->camera->rangeEnd;

	glm::vec3 nearTop = nearCenter + this->camera->up * this->nearHeight;
	glm::vec3 nearBottom = nearCenter - this->camera->up * this->nearHeight;
	glm::vec3 nearLeft = nearCenter - this->camera->right * this->nearWidth;
	glm::vec3 nearRight = nearCenter + this->camera->right * this->nearWidth;
	glm::vec3 farTop = farCenter + this->camera->up * this->farHeight;
	glm::vec3 farBottom = farCenter - this->camera->up * this->farHeight;
	glm::vec3 farLeft = farCenter - this->camera->right * this->farWidth;
	glm::vec3 farRight = farCenter + this->camera->right * this->farWidth;

	glm::vec3 normalTop = glm::normalize(glm::cross(farTop - nearTop, this->camera->right));
	glm::vec3 normalBottom = glm::normalize(glm::cross(this->camera->right, farBottom - nearBottom));
	glm::vec3 normalLeft = glm::normalize(glm::cross(farLeft - nearLeft, this->camera->up));
	glm::vec3 normalRight = glm::normalize(glm::cross(this->camera->up, farRight - nearRight));

	this->frustum[NEAR].point = nearCenter;
	this->frustum[NEAR].normal = this->camera->front;
	this->frustum[FAR].point = farCenter;
	this->frustum[FAR].normal = -this->camera->front;
	this->frustum[TOP].point = nearTop;
	this->frustum[TOP].normal = normalTop;
	this->frustum[BOTTOM].point = nearBottom;
	this->frustum[BOTTOM].normal = normalBottom;
	this->frustum[LEFT].point = nearLeft;
	this->frustum[LEFT].normal = normalLeft;
	this->frustum[RIGHT].point = nearRight;
	this->frustum[RIGHT].normal = normalRight;
}

//-model->meshes.at(j)->radius
void FrustumCulling::cullingSphere(MeshRenderer* meshRenderer) {
	Transform* transform = meshRenderer->gameObject->getComponent<Transform>();

	for (unsigned int meshIndex = 0; meshIndex < meshRenderer->model->meshes.size(); meshIndex++) {

		glm::vec3 boundingMax = glm::vec3(transform->model * glm::vec4(meshRenderer->model->meshes.at(meshIndex)->boundingMax, 1));
		glm::vec3 boundingMin = glm::vec3(transform->model * glm::vec4(meshRenderer->model->meshes.at(meshIndex)->boundingMin, 1));
		glm::vec3 geometryCenter = (boundingMax + boundingMin) * glm::vec3(0.5);
		float radius = glm::length(boundingMax - geometryCenter);

		for(int i = 0; i < 6; i++) {
			if (glm::dot(geometryCenter - this->frustum[i].point, this->frustum[i].normal) < -radius) {
				meshRenderer->culling = true;
				return;
			}
		}
	}
	meshRenderer->culling = false;
}
