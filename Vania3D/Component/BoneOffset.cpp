
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
	this->getBoneOffset();
	this->boneOffset = glm::inverse(this->boneOffset);
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void BoneOffset::update() {
	this->gameObject->transform->position = this->parent->transform->model * this->parent->getComponent<MeshRenderer>()->model->pose.at(this->boneIndex) * this->boneOffset * glm::vec4(0,0,0,1);
}


/*------------------------------------------------------------------------------
< get bone node offset matrix >
------------------------------------------------------------------------------*/
void BoneOffset::getBoneOffset() {
	Node<Bone>* node = this->parent->getComponent<MeshRenderer>()->model->boneNode;
	this->processNode(node);
}

void BoneOffset::processNode(Node<Bone>* node) {
	if (node->data->index == this->boneIndex)
		this->boneOffset =  node->data->offset;
	else
		for (unsigned int i = 0; i < node->children.size(); i++)
			this->processNode(node->children.at(i));
}
