
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
< render shadow >
------------------------------------------------------------------------------*/
void MeshRenderer::renderShadow() {
	// model
	this->gameObject->getComponent<Transform>()->setUniform(this->game->shadowMapping->shader);
	// pose
	this->model->setPoseUniform(this->game->shadowMapping->shader);
	// draw
	this->model->draw();
}
