#include "AudioSource.h"

#include "_SpriteManager.h"
#include "_AudioEngine.h"

#include <sstream>

AudioSource::AudioSource(std::string audioFile, bool looping, bool is3d, float gain, float pitch, float attenuationFactor)
{
	_audioFile = audioFile;
	_bufferID = _AudioEngine::GetInstance()->LoadAudio(audioFile);

	_looping = looping;
	_is3d = is3d;
	_baseGain = _gain = gain;
	_pitch = pitch;
	_attenuationFactor = attenuationFactor;
}

AudioSource::~AudioSource()
{
}

void AudioSource::updateAudioProperties()
{
	if (_sourceID != -1)
	{
		_AudioEngine::GetInstance()->SetAudioProperties(_sourceID, _bufferID, GetGameObject() != nullptr ? glm::vec2(GetGameObject()->GetPosition()) : glm::vec2(), _looping, _is3d, _gain, _pitch,  _attenuationFactor);
	}
}

void AudioSource::play()
{
	if (_sourceID == -1)
	{
		_sourceID = _AudioEngine::GetInstance()->PlayAudio(_bufferID, GetGameObject() != nullptr ? glm::vec2(GetGameObject()->GetPosition()) : glm::vec2(), _looping, _is3d, _gain, _pitch, _attenuationFactor);
	}
	else
	{
		_AudioEngine::GetInstance()->PlayAudioFromSource(_sourceID);
	}
}


void AudioSource::stop() 
{
	_gain = 0.0f;

	_AudioEngine::GetInstance()->StopAudio(_sourceID);
	_sourceID = -1;
}

void AudioSource::pause()
{
	_gain = 0.0f;

	_AudioEngine::GetInstance()->PauseAudio(_sourceID);
}


void AudioSource::Update(double deltaTime)
{
	if (_fadeState != FadeState::NONE)
	{
		if (_fadeState == FadeState::FADING_IN_PLAY)
		{
			_gain += _fadeSpeed * deltaTime;

			if (_gain > _baseGain)
			{
				_gain = _baseGain;
				_fadeState = FadeState::NONE;
			}
		}
		else
		{
			_gain -= _fadeSpeed * deltaTime;

			if (_gain < 0)
			{
				_gain = 0;

				if (_fadeState == FadeState::FADING_OUT_STOP)
				{
					stop();
				}
				else
				{
					pause();
				}

				_fadeState = FadeState::NONE;
			}
		}
	}
	
	updateAudioProperties();


	if (!_looping && !_AudioEngine::GetInstance()->IsPlaying(_sourceID) && !_AudioEngine::GetInstance()->IsPaused(_sourceID))
	{
		stop();
	}
}


void AudioSource::Play(float fadeTime, float offset)
{
	if (fadeTime > 0)
	{
		if (_sourceID == -1)
		{
			_gain = 0;
		}
		_fadeState = FadeState::FADING_IN_PLAY;
		_fadeSpeed = _baseGain / fadeTime;
	}
	else
	{
		_gain = _baseGain;

		_fadeState = FadeState::NONE;
	}

	if (!_AudioEngine::GetInstance()->IsPlaying(_sourceID))
	{
		play();
	}
}

void AudioSource::Replay(float fadeTime, float offset)
{
	Play(fadeTime, offset);
	SetPlaybackTime(0);
}


void AudioSource::Stop(float fadeTime)
{
	if (fadeTime > 0)
	{
		_fadeState = FadeState::FADING_OUT_STOP;
		_fadeSpeed = _gain / fadeTime;
	}
	else
	{
		stop();
	}
}

void AudioSource::Pause(float fadeTime)
{
	if (fadeTime > 0)
	{
		_fadeState = FadeState::FADING_OUT_PAUSE;
		_fadeSpeed = _gain / fadeTime;
	}
	else
	{
		pause();
	}
}

void AudioSource::SetAudioFile(std::string audioFile)
{
	Stop();
	_audioFile = audioFile;
	_bufferID = _AudioEngine::GetInstance()->LoadAudio(audioFile);
}

std::string AudioSource::GetAudioFile()
{
	return _audioFile;
}

void AudioSource::SetLooping(bool looping)
{
	_looping = looping;
}

bool AudioSource::GetLooping()
{
	return _looping;
}

void AudioSource::SetIs3d(bool is3d)
{
	_is3d = is3d;
}

bool AudioSource::GetIs3d()
{
	return _is3d;
}

void AudioSource::SetPitch(float pitch)
{
	_pitch = pitch;
}

float AudioSource::GetPitch()
{
	return _pitch;
}

void AudioSource::SetGain(float gain)
{
	_gain = gain;
	_baseGain = gain;
}

float AudioSource::GetGain()
{
	return _gain;
}

float AudioSource::GetPlaybackTime()
{
	if (_sourceID != -1)
	{
		return _AudioEngine::GetInstance()->GetPlaybackTime(_sourceID);
	}

	return -1;
}

void AudioSource::SetPlaybackTime(float playbackTime)
{
	if (_sourceID != -1)
	{
		_AudioEngine::GetInstance()->SetPlaybackTime(_sourceID, playbackTime);
	}
}

bool AudioSource::IsPlaying()
{
	return _sourceID != -1 && _AudioEngine::GetInstance()->IsPlaying(_sourceID);
}

