

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


void ShaderLayer::add(GameObject* gameObject, unsigned int meshIndex) {
	Transform* transform = gameObject->getComponent<Transform>();
	MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
	
	this->shader = meshRenderer->materials[meshIndex]->shader;
	
	MeshData meshData;
	meshData.transform = transform;
	meshData.model = meshRenderer->model;
	meshData.meshIndex = meshIndex;
	
	if (this->materialLayers.find(meshRenderer->materials[meshIndex]) == this->materialLayers.end()) {
		MaterialLayer* materialLayer = new MaterialLayer();
		materialLayer->meshDatas.push_back(meshData);
		this->materialLayers.insert(std::make_pair(meshRenderer->materials[meshIndex], materialLayer));
	}
	else {
		this->materialLayers[meshRenderer->materials[meshIndex]]->meshDatas.push_back(meshData);
	}
}

void ShaderLayer::render(GameObject* camera) {
	// bind shader
	this->shader->use();
	
	// camera
	camera->getComponent<Camera>()->setUniforms(this->shader);
	
	for (auto it = this->materialLayers.begin(); it != this->materialLayers.end(); it++) {
		
		// texture
		it->first->bindTextures();
		
		for (unsigned int i = 0; i < it->second->meshDatas.size(); i++) {

			// model
			it->second->meshDatas[i].transform->setUniform(this->shader);
			// pose
			for (unsigned int j = 0 ; j < it->second->meshDatas[i].model->pose.size() ; j++) {
				std::string boneName = UNIFORM_MATRIX_BONE;
				this->shader->setMat4((boneName + "[" + std::to_string(j) + "]").c_str(), it->second->meshDatas[i].model->pose[j]);
			}
			// draw
			it->second->meshDatas[i].model->meshes[it->second->meshDatas[i].meshIndex]->draw();
		}

	}
}
