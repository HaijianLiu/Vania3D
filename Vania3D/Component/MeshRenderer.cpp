
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
MeshRenderer::MeshRenderer() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
MeshRenderer::~MeshRenderer() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void MeshRenderer::start() {
	// get main camera from the scene
	this->camera = this->gameObject->scene->mainCamera;
	// initialize static shader uniforms before rendering
	for (unsigned int i = 0; i < this->materials.size(); i++) {
		this->materials[i]->shader->use();
		this->materials[i]->setUniformLocations();
	}
}


/*------------------------------------------------------------------------------
< render model >
------------------------------------------------------------------------------*/
void MeshRenderer::renderModel() {
	// material and shader bind will be refactored!
	for (unsigned int i = 0; i < this->materials.size(); i++) {
		// bind shader
		this->materials[i]->shader->use();
		// camera
		this->materials[i]->shader->setMat4(UNIFORM_MATRIX_PROJECTION, this->camera->getComponent<Camera>()->projection);
		this->materials[i]->shader->setMat4(UNIFORM_MATRIX_VIEW, this->camera->getComponent<Camera>()->view);
		// model
		this->materials[i]->shader->setMat4(UNIFORM_MATRIX_MODEL, this->gameObject->getComponent<Transform>()->model);
		// pose
		for (unsigned int j = 0 ; j < this->model->pose.size() ; j++) {
			std::string boneName = UNIFORM_MATRIX_BONE;
			this->materials[i]->shader->setMat4((boneName + "[" + std::to_string(j) + "]").c_str(), this->model->pose[j]);
		}
		// texture
		this->materials[i]->bindTextures();
		// draw
		this->model->meshes[i]->draw();
	}
}


/*------------------------------------------------------------------------------
< render shadow >
------------------------------------------------------------------------------*/
void MeshRenderer::renderShadow() {
	// model
	this->game->shadowMapping->shader->setMat4(UNIFORM_MATRIX_MODEL, this->gameObject->getComponent<Transform>()->model);
	// pose
	for (unsigned int i = 0 ; i < this->model->pose.size() ; i++) {
		std::string boneName = UNIFORM_MATRIX_BONE;
		this->game->shadowMapping->shader->setMat4((boneName + "[" + std::to_string(i) + "]").c_str(), this->model->pose[i]);
	}
	// draw
	this->model->draw();
}
