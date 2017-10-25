
#ifndef Node_hpp
#define Node_hpp

template <typename T>
class Node {
public:
	std::string name;
	std::vector<Node*> children;
	T data;

	Node(std::string name) {
		this->name = name;
	}
	~Node() {
		deleteVector(this->children);
	}
};

#endif /* Node_hpp */
