#ifndef AUDIO_TEST_SCENE_H
#define AUDIO_TEST_SCENE_H

#include "Scene.h"
#include "AudioSource.h"
#include "Rigidbody.h"
#include "SpriteSheet.h"


class AudioTestScene : public Scene
{
	private:
		//Audio
		GameObject* _audioGameObject;
		AudioSource* _audioSource;

		virtual void onInitialize() override;
		virtual void onTerminate() override;

		virtual void update(double deltaTime) override;
		virtual void fixedUpdate(float fixedTimestep) override;

		virtual void onButtonDown(int button) override;
		virtual void onButtonUp(int button) override;


		virtual void onMouseMove(glm::vec2 position, glm::vec2 worldPosition) override;


	public:

		AudioTestScene();
		~AudioTestScene();
};

REGISTER_SCENE(AudioTestScene);

#endif