#ifndef BENCHMARK_SCENE_H
#define BENCHMARK_SCENE_H

#include "Scene.h"
#include "AudioSource.h"
#include "Rigidbody.h"
#include "SpriteSheet.h"


class BenchmarkScene : public Scene
{
	private:
		//Camera
		Camera* _camera;
		GameObject* _cameraGameObject;

		//Spritesheet
		SpriteSheet* _spriteSheet;

		//Audio
		GameObject* _audioGameObject;
		AudioSource* _audioSource;

		//Background
		GameObject* _backgroundGameObject;

		//GameObjects

		float _goonieMaxScale = 3;
		float _goonieMinScale = 0.5;
		float _goonieBaseSpeed = -100;
		int _nGoonies = 10000; //15.000 a 60fps en Release build, 300 en Debug
		std::vector<GameObject*> _goonies;
		//GameObject testGameObject;
		//Rigidbody* rigidbody;

		//TEMPORAL
		int _testAudio;

		struct CameraButtons { bool moveRight = false, moveLeft = false, moveUp = false, moveDown = false, zoomIn = false, zoomOut = false, rotateCW = false, rotateCCW = false; } _cameraButtons;
		float _cameraMovementSpeed = 100;
		float _cameraZoomSpeed = 0.5;
		float _cameraRotationSpeed = 25;


		virtual void onInitialize() override;
		virtual void onTerminate() override;

		virtual void update(double deltaTime) override;
		virtual void fixedUpdate(float fixedTimestep) override;

		virtual void onButtonDown(int button) override;

		void instanceGoonie(GameObject* goonie);


	public:

		BenchmarkScene();
		~BenchmarkScene();
};

REGISTER_SCENE(BenchmarkScene);

#endif