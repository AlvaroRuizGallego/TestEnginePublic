#include "CameraManualController.h"

#include "InputBinds.h"

CameraManualController::CameraManualController(float movementSpeed, float zoomSpeed, float rotationSpeed)
{
	_cameraMovementSpeed = movementSpeed;
	_cameraZoomSpeed = zoomSpeed;
	_cameraRotationSpeed = rotationSpeed;
}

CameraManualController::~CameraManualController()
{
}

void CameraManualController::OnInitialize()
{
	_camera = _gameObject->GetComponent<Camera>();
}

void CameraManualController::OnButtonDown(int button)
{
	if (button == INPUT_KEY_RIGHT)
	{
		_cameraButtons.moveRight = true;
	}
	if (button == INPUT_KEY_LEFT)
	{
		_cameraButtons.moveLeft = true;
	}
	if (button == INPUT_KEY_UP)
	{
		_cameraButtons.moveUp = true;
	}
	if (button == INPUT_KEY_DOWN)
	{
		_cameraButtons.moveDown = true;
	}
	if (button == INPUT_KEY_D)
	{
		_cameraButtons.rotateCW = true;
	}
	if (button == INPUT_KEY_A)
	{
		_cameraButtons.rotateCCW = true;
	}
	if (button == INPUT_KEY_W)
	{
		_cameraButtons.zoomIn = true;
	}
	if (button == INPUT_KEY_S)
	{
		_cameraButtons.zoomOut = true;
	}
}

void CameraManualController::OnButtonUp(int button)
{
	if (button == INPUT_KEY_RIGHT)
	{
		_cameraButtons.moveRight = false;
	}
	if (button == INPUT_KEY_LEFT)
	{
		_cameraButtons.moveLeft = false;
	}
	if (button == INPUT_KEY_UP)
	{
		_cameraButtons.moveUp = false;
	}
	if (button == INPUT_KEY_DOWN)
	{
		_cameraButtons.moveDown = false;
	}
	if (button == INPUT_KEY_D)
	{
		_cameraButtons.rotateCW = false;
	}
	if (button == INPUT_KEY_A)
	{
		_cameraButtons.rotateCCW = false;
	}
	if (button == INPUT_KEY_W)
	{
		_cameraButtons.zoomIn = false;
	}
	if (button == INPUT_KEY_S)
	{
		_cameraButtons.zoomOut = false;
	}
}

void CameraManualController::Update(double deltaTime)
{
	int cameraHDirection = (_cameraButtons.moveLeft ? -1 : 0) + (_cameraButtons.moveRight ? 1 : 0);
	int cameraVDirection = (_cameraButtons.moveDown ? -1 : 0) + (_cameraButtons.moveUp ? 1 : 0);
	int cameraRotationDirection = (_cameraButtons.rotateCCW ? -1 : 0) + (_cameraButtons.rotateCW ? 1 : 0);
	int cameraZoomDirection = (_cameraButtons.zoomOut ? -1 : 0) + (_cameraButtons.zoomIn ? 1 : 0);

	_gameObject->SetPosition(_gameObject->GetPosition().x + cameraHDirection * _cameraMovementSpeed * deltaTime, _gameObject->GetPosition().y + cameraVDirection * _cameraMovementSpeed * deltaTime);

	_gameObject->SetOrientation(_gameObject->GetOrientation() + glm::radians(cameraRotationDirection* _cameraRotationSpeed * deltaTime));
	_camera->SetZoom(_camera->GetZoom() + cameraZoomDirection * _cameraZoomSpeed * _camera->GetZoom() * deltaTime);
}
