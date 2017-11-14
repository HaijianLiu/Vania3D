

#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
RenderLayer::RenderLayer() {

}


/*------------------------------------------------------------------------------
< Desstructor >
------------------------------------------------------------------------------*/
RenderLayer::~RenderLayer() {

}


/*------------------------------------------------------------------------------
< add >
------------------------------------------------------------------------------*/
void RenderLayer::add(GameObject* gameObject) {
	MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
	for (unsigned int i =0; i < meshRenderer->materials.size(); i++) {
		if (this->shaderLayers.find(meshRenderer->materials[i]->shader->programID) == this->shaderLayers.end()) {
			ShaderLayer* shaderLayer = new ShaderLayer();
			shaderLayer->add(gameObject, i);
			this->shaderLayers.insert(std::make_pair(meshRenderer->materials[i]->shader->programID, shaderLayer));
		}
		else {
			shaderLayers[meshRenderer->materials[i]->shader->programID]->add(gameObject, i);
		}
	}
}


/*------------------------------------------------------------------------------
< render >
------------------------------------------------------------------------------*/
void RenderLayer::render(GameObject* camera) {
	for (auto it = this->shaderLayers.begin(); it != this->shaderLayers.end(); it++) {
		it->second->render(camera);
	}
}


/*------------------------------------------------------------------------------
 < Constructor >
 ------------------------------------------------------------------------------*/
ShaderLayer::ShaderLayer() {

}


/*------------------------------------------------------------------------------
 < Desstructor >
 ------------------------------------------------------------------------------*/
ShaderLayer::~ShaderLayer() {

}


/*------------------------------------------------------------------------------
< add >
------------------------------------------------------------------------------*/
void ShaderLayer::add(GameObject* gameObject, unsigned int meshIndex) {
	Transform* transform = gameObject->getComponent<Transform>();
	MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();

	this->shader = meshRenderer->materials[meshIndex]->shader;

	MeshRenderData* meshRenderData = new MeshRenderData();
	meshRenderData->transform = transform;
	meshRenderData->model = meshRenderer->model;
	meshRenderData->meshIndex = meshIndex;

	if (this->materialLayers.find(meshRenderer->materials[meshIndex]) == this->materialLayers.end()) {
		MaterialLayer* materialLayer = new MaterialLayer();
		materialLayer->meshRenderDatas.push_back(meshRenderData);
		this->materialLayers.insert(std::make_pair(meshRenderer->materials[meshIndex], materialLayer));
	}
	else {
		this->materialLayers[meshRenderer->materials[meshIndex]]->meshRenderDatas.push_back(meshRenderData);
	}
}


/*------------------------------------------------------------------------------
< render >
------------------------------------------------------------------------------*/
void ShaderLayer::render(GameObject* camera) {
	// bind shader
	this->shader->use();
	// camera
	camera->getComponent<Camera>()->setUniforms(this->shader);
	for (auto it = this->materialLayers.begin(); it != this->materialLayers.end(); it++) {
		// texture
		it->first->bindTextures();
		for (unsigned int i = 0; i < it->second->meshRenderDatas.size(); i++) {
			// model
			it->second->meshRenderDatas[i]->transform->setUniform(this->shader);
			// pose
			it->second->meshRenderDatas[i]->model->setPoseUniform(this->shader);
			// draw
			it->second->meshRenderDatas[i]->model->drawMesh(it->second->meshRenderDatas[i]->meshIndex);
		}
	}
}
