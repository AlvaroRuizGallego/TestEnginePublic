#ifndef CAMERA_MANUAL_CONTROLLER_H
#define CAMERA_MANUAL_CONTROLLER_H

#include "Component.h"
#include "Camera.h"

class CameraManualController : public Component
{
	private:
		struct CameraButtons { bool moveRight = false, moveLeft = false, moveUp = false, moveDown = false, zoomIn = false, zoomOut = false, rotateCW = false, rotateCCW = false; } _cameraButtons;
		float _cameraMovementSpeed;
		float _cameraZoomSpeed;
		float _cameraRotationSpeed;
		Camera* _camera;

	public:
		CameraManualController(float movementSpeed = 100, float zoomSpeed = 0.5, float rotationSpeed = 25);
		~CameraManualController();

		void OnInitialize() override;
		void OnButtonDown(int button) override;
		void OnButtonUp(int button) override;
		void Update(double deltaTime) override;
};


#endif
