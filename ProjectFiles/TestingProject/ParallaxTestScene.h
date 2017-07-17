#ifndef PARALLAX_TEST_SCENE_H
#define PARALLAX_TEST_SCENE_H

#include "Scene.h"
#include "AudioSource.h"
#include "SpriteSheet.h"


class ParallaxTestScene : public Scene
{
	private:
		//Camera
		Camera* _camera;

		//Spritesheet
		SpriteSheet* _spriteSheet;


		GameObject* _gameObject;
		GameObject* _cameraGameObject;
		GameObject* _backgroundGameObject;


		//TEMPORAL
		int _testAudio;

		struct CameraButtons { bool moveRight = false, moveLeft = false, moveUp = false, moveDown = false, zoomIn = false, zoomOut = false, rotateCW = false, rotateCCW = false; } _cameraButtons;
		float _cameraMovementSpeed = 300;
		float _cameraZoomSpeed = 1.5;
		float _cameraRotationSpeed = glm::radians(75.0f);


		virtual void onInitialize() override;
		virtual void onTerminate() override;

		virtual void update(double deltaTime) override;
		virtual void fixedUpdate(float fixedTimestep) override;


		virtual void onButtonDown(int button) override;
		virtual void onButtonUp(int button) override;
		virtual void onControllerButtonDown(int controller, int button) override;
		virtual void onControllerButtonUp(int controller, int button) override;
		virtual void onMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition) override;
		virtual void onMouseButtonUp(int button, glm::vec2 position, glm::vec2 worldPosition) override;
		virtual void onMouseMove(glm::vec2 position, glm::vec2 worldPosition) override;


	public:

		ParallaxTestScene();
		~ParallaxTestScene();
};

REGISTER_SCENE(ParallaxTestScene);

#endif