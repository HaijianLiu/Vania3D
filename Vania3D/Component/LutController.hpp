
#ifndef LutController_hpp
#define LutController_hpp

class LutController : public Component {
private:
	std::vector<unsigned int> luts;
	int currentLutIndex = 0;

	void start();
	void update();

public:
	LutController();
	~LutController();
};

#endif /* LutController_hpp */
