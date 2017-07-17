#ifndef AUDIO_TEST_SCENE_2_H
#define AUDIO_TEST_SCENE_2_H

#include "Scene.h"
#include "AudioSource.h"
#include "Rigidbody.h"
#include "SpriteSheet.h"


class AudioTestScene2 : public Scene
{
	private:
		//Camera
		GameObject* _cameraGameObject;

		bool moveCameraLeft = false;
		bool moveCameraRight = false;
		bool moveCameraUp = false;
		bool moveCameraDown = false;

		virtual void onInitialize() override;
		virtual void onTerminate() override;

		virtual void update(double deltaTime) override;
		virtual void fixedUpdate(float fixedTimestep) override;

		virtual void onButtonDown(int button) override;
		void onMouseButtonDown(int button, glm::vec2 screenPosition, glm::vec2 worldPosition) override;

	public:

		AudioTestScene2();
		~AudioTestScene2();
};

REGISTER_SCENE(AudioTestScene2);

#endif