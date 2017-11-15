

#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
RenderLayer::RenderLayer() {

}

ShaderLayer::ShaderLayer() {

}

MaterialLayer::MaterialLayer() {

}


/*------------------------------------------------------------------------------
< Desstructor >
------------------------------------------------------------------------------*/
RenderLayer::~RenderLayer() {
	deleteMap(this->shaderLayers);
}

ShaderLayer::~ShaderLayer() {
	deleteMap(this->materialLayers);
	deleteMap(this->materialLayersTwoSides);
}

MaterialLayer::~MaterialLayer() {

}


/*------------------------------------------------------------------------------
< add >
------------------------------------------------------------------------------*/
void RenderLayer::add(GameObject* gameObject) {
	MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();

	for (unsigned int i = 0; i < meshRenderer->materials.size(); i++) {
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

void ShaderLayer::add(GameObject* gameObject, unsigned int meshIndex) {
	MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();

	this->shader = meshRenderer->materials[meshIndex]->shader;

	if (meshRenderer->materials[meshIndex]->twoSides) {
		if (this->materialLayersTwoSides.find(meshRenderer->materials[meshIndex]) == this->materialLayersTwoSides.end()) {
			MaterialLayer* materialLayer = new MaterialLayer();
			materialLayer->add(gameObject, meshIndex);
			this->materialLayersTwoSides.insert(std::make_pair(meshRenderer->materials[meshIndex], materialLayer));
		}
		else {
			this->materialLayersTwoSides[meshRenderer->materials[meshIndex]]->add(gameObject, meshIndex);
		}
	}
	else {
		if (this->materialLayers.find(meshRenderer->materials[meshIndex]) == this->materialLayers.end()) {
			MaterialLayer* materialLayer = new MaterialLayer();
			materialLayer->add(gameObject, meshIndex);
			this->materialLayers.insert(std::make_pair(meshRenderer->materials[meshIndex], materialLayer));
		}
		else {
			this->materialLayers[meshRenderer->materials[meshIndex]]->add(gameObject, meshIndex);
		}
	}
}

void MaterialLayer::add(GameObject* gameObject, unsigned int meshIndex) {
	Mesh* mesh = gameObject->getComponent<MeshRenderer>()->model->meshes[meshIndex];
	
	if (this->meshRenderDatas.find(mesh) == this->meshRenderDatas.end()) {
		MeshRenderData* meshRenderData = new MeshRenderData();
		meshRenderData->gameObjects.push_back(gameObject);
		this->meshRenderDatas.insert(std::make_pair(mesh, meshRenderData));
	}
	else {
		this->meshRenderDatas.at(mesh)->gameObjects.push_back(gameObject);
	}
}


/*------------------------------------------------------------------------------
< render >
------------------------------------------------------------------------------*/
void RenderLayer::render(GameObject* camera) {
	for (auto it = this->shaderLayers.begin(); it != this->shaderLayers.end(); it++)
		it->second->render(camera);
}

void ShaderLayer::render(GameObject* camera) {
	// bind shader
	this->shader->use();
	// set camera uniforms
	camera->getComponent<Camera>()->setUniforms(this->shader);
	for (auto it = this->materialLayers.begin(); it != this->materialLayers.end(); it++) {
		// bind textures
		it->first->bindTextures();
		it->second->render(this->shader);
	}

	if (this->materialLayersTwoSides.size() > 0) {
		glDisable(GL_CULL_FACE);
		for (auto it = this->materialLayersTwoSides.begin(); it != this->materialLayersTwoSides.end(); it++) {
			// bind textures
			it->first->bindTextures();
			it->second->render(this->shader);
		}
		glEnable(GL_CULL_FACE);
	}
}

void MaterialLayer::render(Shader* shader) {
	for (auto it = this->meshRenderDatas.begin(); it != this->meshRenderDatas.end(); it++) {
		// bind vao
		glBindVertexArray(it->first->vao);
		for (unsigned int i = 0; i < it->second->gameObjects.size(); i++) {
			MeshRenderer* meshRenderer = it->second->gameObjects[i]->getComponent<MeshRenderer>();
			if (!meshRenderer->culling) {
				Transform* transform = it->second->gameObjects[i]->getComponent<Transform>();
				// model
				transform->setUniform(shader);
				// pose
				meshRenderer->model->setPoseUniform(shader);
				// draw
				it->first->draw();
			}
		}
		glBindVertexArray(0);
	}
}
