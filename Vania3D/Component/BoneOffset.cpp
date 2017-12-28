
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
BoneOffset::BoneOffset() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
BoneOffset::~BoneOffset() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void BoneOffset::start() {
	this->model = this->parent->getComponent<MeshRenderer>()->model;
	Node<Bone>* node = this->model->boneNode;
	this->processNode(node);
	this->boneOffset = glm::inverse(this->boneOffset);
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void BoneOffset::update() {
	this->gameObject->transform->position = this->parent->transform->model * this->model->pose.at(this->boneIndex) * this->boneOffset * glm::vec4(0,0,0,1);
}


/*------------------------------------------------------------------------------
< process bone node to find bone offset matrix >
------------------------------------------------------------------------------*/
void BoneOffset::processNode(Node<Bone>* node) {
	if (node->name == this->boneName) {
		this->boneOffset = node->data->offset;
		this->boneIndex = node->data->index;
	}
	else
		for (unsigned int i = 0; i < node->children.size(); i++)
			this->processNode(node->children.at(i));
}
