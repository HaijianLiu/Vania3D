
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Node::Node(std::string name) {
	this->name = name;
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Node::~Node() {
	deleteVector(this->children);
}
