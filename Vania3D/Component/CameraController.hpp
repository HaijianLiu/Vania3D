
#ifndef CameraController_hpp
#define CameraController_hpp

class CameraController : public Component {
public:
	GameObject* camera;

	CameraController();
	~CameraController();

	void update();
};

#endif /* CameraController_hpp */
