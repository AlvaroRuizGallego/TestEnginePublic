#include "AudioListener.h"

#include "_AudioEngine.h"



bool AudioListener::_existsListener = false;

AudioListener::AudioListener()
{
}

AudioListener::~AudioListener()
{
}

void AudioListener::OnInitialize()
{
	_AudioEngine::GetInstance()->SetListenerProperties(glm::vec2(_gameObject->GetPosition()), _gameObject->GetOrientation());

	_previousPosition = glm::vec2(_gameObject->GetPosition());
	_previousOrientation = _gameObject->GetOrientation();

	if (_existsListener)
	{
		_activeListener = false;

		printf("\nWARNING: AudioListener::OnInitialize -> Already exists a listener");
	}
	else
	{
		_existsListener = true;
		_activeListener = true;
	}
}

void AudioListener::OnTerminate()
{
	if (_activeListener)
	{
		_existsListener = false;
	}
}

void AudioListener::OnRemovedFromGameObject()
{
	if (_activeListener)
	{
		_existsListener = false;
	}
}

void AudioListener::Update(double deltaTime)
{
	if (_activeListener)
	{
		if (_previousPosition.x != _gameObject->GetPosition().x || _previousPosition.y != _gameObject->GetPosition().y || _previousOrientation != _gameObject->GetOrientation())
		{
			_AudioEngine::GetInstance()->SetListenerProperties(glm::vec2(_gameObject->GetPosition()), _gameObject->GetOrientation());

			_previousPosition = glm::vec2(_gameObject->GetPosition());
			_previousOrientation = _gameObject->GetOrientation();
		}
	}
}

