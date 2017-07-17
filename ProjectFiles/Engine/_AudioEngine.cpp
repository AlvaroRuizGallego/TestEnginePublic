#include "_AudioEngine.h"

#include "_AudioLoader.h"
#include "Camera.h"

#include "al.h"
#include "alc.h"

#include <iostream>
#include <fstream>
#include <cstring>


_AudioEngine* _AudioEngine::_instance = nullptr;


_AudioEngine::_AudioEngine()
{
	_nBuffersUsed = 0;

	initializeAL();
	createBuffers();
	createSources();
	initializeListener();
}

void _AudioEngine::checkForErrors(std::string  debugText)
{
	int error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("\n--------ERROR %i--------\n", error);

		switch (error)
		{
		case AL_NO_ERROR:
			printf("\nAL_NO_ERROR");
			break;

		case AL_INVALID_OPERATION:
			printf("\nAL_INVALID_OPERATION");
			break;

		case AL_INVALID_ENUM:
			printf("\nAL_INVALID_ENUM");
			break;

		case AL_INVALID_VALUE:
			printf("\nAL_INVALID_VALUE");
			break;

		case AL_OUT_OF_MEMORY:
			printf("\nAL_OUT_OF_MEMORY");
			break;

		case AL_INVALID_NAME:
			printf("\nAL_INVALID_NAME");
			break;

		}
		printf("\n");
		printf(debugText.c_str());
		printf("\n");
	}
}

void _AudioEngine::initializeAL()
{
	alGetError();

	_device = alcOpenDevice(NULL);
	if (!_device)
	{
		printf("\nERROR: _AudioEngine::initializeAL -> No device found");
	}
	
	_context = alcCreateContext((ALCdevice*)_device, NULL);

	if (!alcMakeContextCurrent((ALCcontext*)_context))
	{
		printf("\nERROR: _AudioEngine::initializeAL -> Failed to create context");
		alcDestroyContext((ALCcontext*)_context);
		alcCloseDevice((ALCdevice*)_device);
	}

	alDistanceModel(AL_NONE);
}


void _AudioEngine::initializeListener()
{
	_listenerZPosition = 0;
	SetListenerProperties(glm::vec2(0, 0), 0);
}

void _AudioEngine::createSources()
{
	alGenSources((unsigned int)N_SOURCES, _sources);
}

void _AudioEngine::createBuffers()
{
	alGenBuffers((unsigned int)N_BUFFERS, _buffers);
}


int _AudioEngine::getFreeSource()
{
	int sourceState;

	for (unsigned int sourceID : _sources)
	{
		alGetSourcei(sourceID, AL_SOURCE_STATE, &sourceState);

		if (sourceState != AL_PLAYING && sourceState != AL_PAUSED)
		{
			clearSourceProperties(sourceID);
			return sourceID;
		}
	}

	return -1;
}

void _AudioEngine::setSource3dProperties(unsigned int sourceID, _Source3dProperties source3dProperties)
{
	float listenerPos[3];
	alGetListenerfv(AL_POSITION, &listenerPos[0]);
	glm::vec2 listenerPosition = glm::vec2(listenerPos[0], listenerPos[1]);

	if (source3dProperties.is3d)
	{
		//Attenuation
		if (source3dProperties.attenuationFactor == 0)
		{
			alSourcef(sourceID, AL_GAIN, source3dProperties.gain);
		}
		else
		{
			float attenuationDistance = (_maxAttenuationDistance - _minAttenuationDistance) / source3dProperties.attenuationFactor;
			float distance = glm::clamp(glm::distance(source3dProperties.position, listenerPosition), _minAttenuationDistance, _minAttenuationDistance + attenuationDistance);

			alSourcef(sourceID, AL_GAIN, source3dProperties.gain * (1 - (distance - _minAttenuationDistance) / (attenuationDistance)));
		}

		//Panning
		float positionX = (source3dProperties.position.x - listenerPosition.x);

		if (positionX < 0)
		{
			positionX = glm::clamp(positionX, -_maxPanningDistance, -_minPanningDistance);
			positionX += _minPanningDistance;
		}
		else
		{
			positionX = glm::clamp(positionX, _minPanningDistance, _maxPanningDistance);
			positionX -= _minPanningDistance;
		}

		alSource3f(sourceID, AL_POSITION, listenerPosition.x + positionX, source3dProperties.position.y, 0);
	}
	else
	{
		alSourcef(sourceID, AL_GAIN, source3dProperties.gain);
		alSource3f(sourceID, AL_POSITION, listenerPosition.x, listenerPosition.y, 0);
	}

	//Update map
	_source3dPropertiesMap[sourceID] = source3dProperties;
}


void _AudioEngine::clearSourceProperties(unsigned int sourceID)
{
	StopAudio(sourceID);

	int nBuffersProcessed;
	alGetSourcei(sourceID, AL_BUFFERS_PROCESSED, &nBuffersProcessed);

	if (nBuffersProcessed > 0)
	{
		unsigned int buf;
		alSourceUnqueueBuffers(sourceID, nBuffersProcessed, &buf);
	}

	alSourcei(sourceID, AL_BUFFER, NULL);

	alSourcef(sourceID, AL_PITCH, 1);
	alSourcef(sourceID, AL_GAIN, 1);
	alSource3f(sourceID, AL_POSITION, 0, 0, 0);
	alSource3f(sourceID, AL_VELOCITY, 0, 0, 0);
	alSourcei(sourceID, AL_LOOPING, AL_TRUE);
}

void _AudioEngine::updateListenerZPosition()
{
	_listenerZPosition = (_maxPanningDistance - _minPanningDistance) * 1.85;

	float listenerPos[3];
	alGetListenerfv(AL_POSITION, &listenerPos[0]);
	listenerPos[2] = _listenerZPosition;
	alListenerfv(AL_POSITION, listenerPos);
}


void _AudioEngine::SetParameters(float masterGain, float minAttenuationDistance, float maxAttenuationDistance, float minPanningDistance, float maxPanningDistance)
{
	SetMasterGain(masterGain);
	SetDistances(minAttenuationDistance, maxAttenuationDistance, minPanningDistance, maxPanningDistance);
}

void _AudioEngine::Clear()
{
	for (unsigned int sourceID : _sources)
	{
		clearSourceProperties(sourceID);
	}

	alDeleteBuffers(N_BUFFERS, _buffers);
	createBuffers();

	_audioMap.clear();
	_source3dPropertiesMap.clear();

	_nBuffersUsed = 0;
}


int _AudioEngine::LoadAudio(const std::string filename)
{
	auto it = _audioMap.find(std::string(filename));
	if (it != _audioMap.end())
	{
		return it->second;
	}
	
	if (_nBuffersUsed >= N_BUFFERS)
	{
		printf("\nWARNING: _AudioEngine::LoadAudio -> Cannot load audio. Not enough buffers");
		return -1;
	}

	std::string formatStr;

	for (int i = filename.length() - 1; i >= 0; --i)
	{
		if (filename.at(i) == '.')
		{
			formatStr = filename.substr(i + 1);
		}
	}

	ALbyte *data;
	int format;
	int size, frequency;

	if (formatStr == "wav")
	{
		data = _AudioLoader::GetInstance()->LoadWAV(filename.c_str(), format, size, frequency);
	}
	else
	{
		printf("\nWARNING: _AudioEngine::LoadAudio -> Format not valid. Please load a .wav file.");
		return -1;
	}

	unsigned int bufferID = _buffers[_nBuffersUsed++];
	
	alBufferData(bufferID, format, (void*)data, size, frequency);

	_audioMap.insert(std::pair<std::string, unsigned int>(filename, bufferID));

	delete[] data;

	return bufferID;
}


int _AudioEngine::PlayAudio(unsigned int bufferID, glm::vec2 position, bool looping, bool is3d, float gain, float pitch, float attenuationFactor)
{
	int sourceID = getFreeSource();

	if (sourceID != -1)
	{
		SetAudioProperties(sourceID, bufferID, position, looping, is3d, gain, pitch, attenuationFactor);
		PlayAudioFromSource(sourceID);
	}

	return sourceID;
}

void _AudioEngine::PlayAudioFromSource(unsigned int sourceID)
{
	if (sourceID != -1)
	{
		alSourcePlay(sourceID);
	}
}


void _AudioEngine::StopAudio(unsigned int sourceID)
{
	if (sourceID != -1)
	{
		alSourceStop(sourceID);
	}
}

void _AudioEngine::PauseAudio(unsigned int sourceID)
{
	if (sourceID != -1)
	{
		alSourcePause(sourceID);
	}
}

void _AudioEngine::SetAudioProperties(unsigned int sourceID, unsigned int bufferID, glm::vec2 position, bool looping, bool is3d, float gain, float pitch, float attenuationFactor)
{
	int nBuffers;
	alGetSourcei(sourceID, AL_BUFFERS_QUEUED, &nBuffers);
	if (nBuffers == 0)
	{
		alSourcei(sourceID, AL_BUFFER, bufferID);
	}

	alSourcef(sourceID, AL_PITCH, pitch);
	alSourcei(sourceID, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);

	setSource3dProperties(sourceID, { is3d, position, gain, attenuationFactor });

	ALfloat gain2;
	alGetSourcef(sourceID, AL_GAIN, &gain2);
}

float _AudioEngine::GetPlaybackTime(unsigned int sourceID)
{
	float playbackTime;
	alGetSourcef(sourceID, AL_SEC_OFFSET, &playbackTime);

	return playbackTime;
}

void _AudioEngine::SetPlaybackTime(unsigned int sourceID, float playbackTime)
{
	alSourcef(sourceID, AL_SEC_OFFSET, playbackTime);
}

void _AudioEngine::SetMasterGain(float gain)
{
	alListenerf(AL_GAIN, gain);
}

float _AudioEngine::GetMasterGain()
{
	float gain;
	alGetListenerf(AL_GAIN, &gain);

	return gain;
}

void _AudioEngine::SetListenerProperties(glm::vec2 position, float orientation)
{
	float listenerPosition[] = { position.x, position.y, _listenerZPosition };
	float listenerOrientation[] = { 0.0f, 0.0f, -1.0f, glm::cos(orientation - glm::radians(90.0f)), -glm::sin(orientation - glm::radians(90.0f)), 0.0f };

	alListenerfv(AL_POSITION, listenerPosition);
	alListenerfv(AL_ORIENTATION, listenerOrientation);

	int sourceState;

	for (unsigned int sourceID : _sources)
	{
		alGetSourcei(sourceID, AL_SOURCE_STATE, &sourceState);

		if (sourceState == AL_PLAYING || sourceState == AL_PAUSED)
		{
			if (_source3dPropertiesMap[sourceID].is3d)
			{
				setSource3dProperties(sourceID, _source3dPropertiesMap[sourceID]);
			}
		}
	}
}

void _AudioEngine::SetDistances(float minAttenuationDistance, float maxAttenuationDIstance, float minPanningDistance, float maxPanningDistance)
{
	_minAttenuationDistance = minAttenuationDistance;
	_maxAttenuationDistance = maxAttenuationDIstance;
	_minPanningDistance = minPanningDistance;
	_maxPanningDistance = maxPanningDistance;

	updateListenerZPosition();
}

void _AudioEngine::SetMinAttenuationDistance(float distance)
{
	_minAttenuationDistance = distance;
	updateListenerZPosition();
}

float _AudioEngine::GetMinAttenuationDistance()
{
	return _minAttenuationDistance;
}

void _AudioEngine::SetMaxAttenuationDistance(float distance)
{
	_maxAttenuationDistance = distance;
	updateListenerZPosition();
}

float _AudioEngine::GetMaxAttenuationDistance()
{
	return _maxAttenuationDistance;
}

void _AudioEngine::SetMinPanningDistance(float distance)
{
	_minPanningDistance = distance;
	updateListenerZPosition();
}

float _AudioEngine::GetMinPanningDistance()
{
	return _minPanningDistance;
}

void _AudioEngine::SetMaxPanningDistance(float distance)
{
	_maxPanningDistance = distance;
	updateListenerZPosition();
}

float _AudioEngine::GetMaxPanningDistance()
{
	return _maxPanningDistance;
}

bool _AudioEngine::IsPlaying(unsigned int sourceID)
{
	int state;
	alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}

bool _AudioEngine::IsPaused(unsigned int sourceID)
{
	int state;
	alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
	return (state == AL_PAUSED); return false;
}


_AudioEngine::~_AudioEngine()
{
	_instance = nullptr;

	alDeleteSources(N_SOURCES, _sources);
	alDeleteBuffers(N_BUFFERS, _buffers);
	alcMakeContextCurrent(NULL);
	alcDestroyContext((ALCcontext*)_context);
	alcCloseDevice((ALCdevice*)_device);
}

_AudioEngine * _AudioEngine::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new _AudioEngine();
	}
	return _instance;
}


