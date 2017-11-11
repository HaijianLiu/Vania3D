
#ifndef CameraController_hpp
#define CameraController_hpp

class CameraController : public Component {
private:
	glm::vec3 offsetFromTarget; // camera offset from target's offset

	void start();
	void update();

	void rotate(float radiansHorizonal, float radiansVertical);
	void zoom(float distance);

public:
	GameObject* camera;

	// properties
	int inverseHorizonal = CAMERA_INVERSE_OFF;
	int inverseVertical = CAMERA_INVERSE_ON;
	float rotateSensitivityHorizonal = 2;
	float rotateSensitivityVertical = 1;
	float zoomSensitivity = 6;
	float offsetFromTargetMin = 100;
	float offsetFromTargetMax = 500;
	float verticalRadiansMin = 0;
	float verticalRadiansMax = 0.8;

	CameraController();
	~CameraController();
};

#endif /* CameraController_hpp */
