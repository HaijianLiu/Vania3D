
#ifndef Node_hpp
#define Node_hpp

class Node {
public:
	std::string name;
	aiMatrix4x4 transformation;
	std::vector<Node*> children;

	Node(std::string name);
	~Node();
};

#endif /* Node_hpp */
