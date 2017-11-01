
#ifndef CameraController_hpp
#define CameraController_hpp

class CameraController {
private:

public:
	Camera* camera;
	Transform* target;


	Transform* transform;


	CameraController();
	~CameraController();

	void update();
};

#endif /* CameraController_hpp */
