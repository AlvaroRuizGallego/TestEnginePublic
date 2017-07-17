#ifndef AUDIO_LISTENER_H
#define AUDIO_LISTENER_H

#include "Component.h"

#include <glm.hpp>

class __declspec(dllexport) AudioListener: public Component
{
	private:
		glm::vec2 _previousPosition;
		float _previousOrientation;
		bool _activeListener;

		static bool _existsListener;

	public:
		AudioListener();
		~AudioListener();

		void OnInitialize() override;
		void OnTerminate() override;
		void OnRemovedFromGameObject() override;
		void Update(double deltaTime) override;
};

#endif

