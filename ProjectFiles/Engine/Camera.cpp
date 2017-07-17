#include "Camera.h"

#include "_RenderingEngine.h"

#include "gtc\matrix_transform.hpp"
#include "gtc\matrix_inverse.hpp""

#include <iostream>

Camera* Camera::_activeCamera = nullptr;
int Camera::_windowWidth;
int Camera::_windowHeight;
int Camera::_referenceWindowWidth;
int Camera::_referenceWindowHeight;
float Camera::_maxDepth;
Camera::_ScreenFitMode  Camera::_screenFitMode;

void Camera::setActiveCamera(Camera* camera)
{
	_activeCamera = camera;
}

glm::mat4 Camera::getOrthoTransform(float zoom)
{
	return  glm::ortho(-(_referenceWindowWidth / 2) / zoom, (_referenceWindowWidth / 2) / zoom, -(_referenceWindowHeight / 2) / zoom, (_referenceWindowHeight / 2) / zoom, _maxDepth, -_maxDepth);
}


Camera::Camera(float zoom)
{
	_transform = glm::mat4();
	SetZoom(zoom);
	_cameraSpaceTransform = getOrthoTransform(1);
	_vignette = nullptr;
}

Camera::~Camera()
{
}

Camera * Camera::SetZoom(float zoom)
{
	_zoom = zoom;

	_isTransformDirty = true;

	return this;
}


float Camera::GetZoom()
{
	return _zoom;
}

glm::mat4 Camera::GetCameraSpaceTransform()
{
	return _cameraSpaceTransform;
}

glm::mat4 Camera::GetTransform()
{
	if (_isTransformDirty || (_gameObject != nullptr && _gameObject->IsTransformDirty()))
	{
		float orientation = 0;
		glm::vec3 position = glm::vec3();

		if (_gameObject != nullptr)
		{
			orientation = _gameObject->GetOrientation();
			position = _gameObject->GetPosition();
			position.z = 0;
		}

		glm::vec3 up = glm::vec3(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) * glm::rotate(glm::mat4(), - orientation, glm::vec3(0.0f, 0.0f, -1.0f)));
		_transform = getOrthoTransform(_zoom) * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), up);

		_isTransformDirty = false;
	}

	return _transform;
}

void Camera::OnInitialize()
{
	_RenderingEngine::GetInstance()->SetCamera(this);
}

Camera * Camera::GetActiveCamera()
{
	return _activeCamera;
}

glm::vec2 Camera::ScreenToWorldPosition(glm::vec2 screenPosition)
{
	glm::vec2 cameraSpacePosition = ScreenToCameraPosition(screenPosition);
	glm::vec2 relativeScreenPosition = glm::vec2(cameraSpacePosition.x / (_windowWidth * 0.5f), cameraSpacePosition.y / (_windowHeight * 0.5f));

	Rectangle screenRegion = GetScreenRegion();
	relativeScreenPosition.x *= _windowWidth / screenRegion.GetWidth();
	relativeScreenPosition.y *= _windowHeight / screenRegion.GetHeight();

	glm::vec2 worldSpacePosition = glm::vec2(glm::inverse(GetTransform()) * glm::vec4(relativeScreenPosition.x, relativeScreenPosition.y, 0, 1));

	return worldSpacePosition;
}

glm::vec2 Camera::WorldToScreenPosition(glm::vec2 worldPosition)
{
	glm::vec2 relativeScreenPosition = glm::vec2(GetTransform() * glm::vec4(worldPosition.x, worldPosition.y, 0, 1));
	
	Rectangle screenRegion = GetScreenRegion();
	relativeScreenPosition.x /= _windowWidth / screenRegion.GetWidth();
	relativeScreenPosition.y /= _windowHeight / screenRegion.GetHeight();

	glm::vec2 cameraSpacePosition = glm::vec2(relativeScreenPosition.x * _windowWidth*0.5, relativeScreenPosition.y * _windowHeight*0.5);
	glm::vec2 screenPosition = CameraToScreenPosition(cameraSpacePosition);

	return screenPosition;
}

glm::vec2 Camera::CameraToWorldPosition(glm::vec2 cameraPosition)
{
	return ScreenToWorldPosition(CameraToScreenPosition(cameraPosition));
}

glm::vec2 Camera::WorldToCameraPosition(glm::vec2 worldPosition)
{
	return ScreenToCameraPosition(WorldToScreenPosition(worldPosition));
}

glm::vec2 Camera::ScreenToCameraPosition(glm::vec2 screenPosition)
{
	return screenPosition - glm::vec2(_windowWidth * 0.5, _windowHeight*0.5);
}

glm::vec2 Camera::CameraToScreenPosition(glm::vec2 cameraPosition)
{
	return cameraPosition + glm::vec2(_windowWidth * 0.5, _windowHeight*0.5);
}

Rectangle Camera::GetScreenRegion()
{
	Rectangle screenRegion;

	if (_screenFitMode == _ScreenFitMode::EXPAND)
	{
		screenRegion = Rectangle(0, 0, _windowWidth, _windowHeight);
	}
	else
	{
		if (_screenFitMode == _ScreenFitMode::WIDTH_FIT || (_screenFitMode == _ScreenFitMode::BEST_FIT && (float)_windowWidth / _referenceWindowWidth < (float)_windowHeight / _referenceWindowHeight))
		{
			glm::vec2 minCameraSpace = glm::vec2(-(float)_windowWidth * 0.5, -((float)_windowWidth / (float)_referenceWindowWidth) * (float)_referenceWindowHeight * 0.5);
			glm::vec2 maxCameraSpace = -minCameraSpace;
			screenRegion = Rectangle(CameraToScreenPosition(minCameraSpace), CameraToScreenPosition(maxCameraSpace));
		}
		else
		{
			glm::vec2 minCameraSpace = glm::vec2(-((float)_windowHeight / (float)_referenceWindowHeight) * (float)_referenceWindowWidth * 0.5, -(float)_windowHeight * 0.5);
			glm::vec2 maxCameraSpace = -minCameraSpace;
			screenRegion = Rectangle(CameraToScreenPosition(minCameraSpace), CameraToScreenPosition(maxCameraSpace));
		}
	}

	return screenRegion;
}

void Camera::SetVignette(std::string imagePath, Rectangle uvs)
{
	_vignette = new _Sprite(imagePath, true, uvs);
	_vignette->SetScale(_referenceWindowWidth / _vignette->GetSize().x, _referenceWindowHeight / _vignette->GetSize().y);
}

void Camera::RemoveVignette()
{
	delete _vignette;
	_vignette = nullptr;
}

void Camera::SetVignetteColor(glm::vec4 color)
{
	if (_vignette != nullptr)
	{
		_vignette->SetColor(color);
	}
}

glm::vec4 Camera::GetVignetteColor()
{
	if (_vignette != nullptr)
	{
		return _vignette->GetColor();
	}

	return glm::vec4();
}
