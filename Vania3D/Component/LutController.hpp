
#ifndef LutController_hpp
#define LutController_hpp

class LutController {
private:
	std::vector<unsigned int> luts;

	void start();
	void update();

public:
	LutController();
	~LutController();
};

#endif /* LutController_hpp */
