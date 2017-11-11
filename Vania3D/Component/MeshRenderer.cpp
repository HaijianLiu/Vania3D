
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
	this->material->shader->use();
	this->material->setUniformLocations();
}


/*------------------------------------------------------------------------------
< render model >
------------------------------------------------------------------------------*/
void MeshRenderer::renderModel() {
	// bind shader
	this->material->shader->use();
	// camera
	this->material->shader->setMat4(UNIFORM_MATRIX_PROJECTION, this->camera->getComponent<Camera>()->projection);
	this->material->shader->setMat4(UNIFORM_MATRIX_VIEW, this->camera->getComponent<Camera>()->view);
	// model
	this->material->shader->setMat4(UNIFORM_MATRIX_MODEL, this->gameObject->getComponent<Transform>()->model);
	// pose
	for (unsigned int i = 0 ; i < this->model->pose.size() ; i++) {
		std::string boneName = UNIFORM_MATRIX_BONE;
		this->material->shader->setMat4((boneName + "[" + std::to_string(i) + "]").c_str(), this->model->pose[i]);
	}
	// texture
	this->material->bindTextures();
	// draw
	this->model->draw();
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
