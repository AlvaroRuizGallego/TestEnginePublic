#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "Geometry.h"
#include "_Sprite.h"

#include "glm.hpp"

class _RenderingEngine;

class __declspec(dllexport) Camera: public Component
{
	friend class _RenderingEngine;
	friend class Game;

	private:

		enum _ScreenFitMode { BEST_FIT, WIDTH_FIT, HEIGHT_FIT, EXPAND };

		float _zoom;
		glm::mat4 _transform;
		glm::mat4 _cameraSpaceTransform;
		bool _isTransformDirty;

		_Sprite* _vignette;

		static Camera* _activeCamera;
		static void setActiveCamera(Camera* camera);

		static int _windowWidth;
		static int _windowHeight;
		static int _referenceWindowWidth;
		static int _referenceWindowHeight;
		static float _maxDepth;
		static _ScreenFitMode _screenFitMode;

		glm::mat4 getOrthoTransform(float zoom);


	public:

		Camera(float zoom = 1);
		~Camera();

		Camera* SetZoom(float zoom);

		float GetZoom();

		glm::mat4 GetCameraSpaceTransform();
		glm::mat4 GetTransform();

		virtual void OnInitialize();

		static Camera* GetActiveCamera();

		glm::vec2 ScreenToWorldPosition(glm::vec2 screenPosition);
		glm::vec2 WorldToScreenPosition(glm::vec2 worldPosition);
		glm::vec2 CameraToWorldPosition(glm::vec2 cameraPosition);
		glm::vec2 WorldToCameraPosition(glm::vec2 worldPosition);
		glm::vec2 ScreenToCameraPosition(glm::vec2 screenPosition);
		glm::vec2 CameraToScreenPosition(glm::vec2 cameraPosition);

		Rectangle GetScreenRegion();

		void SetVignette(std::string imagePath, Rectangle uvs = Rectangle({ 0,0 }, { 1,1 }));
		void RemoveVignette();
		void SetVignetteColor(glm::vec4 color);
		glm::vec4 GetVignetteColor();
};

#endif

