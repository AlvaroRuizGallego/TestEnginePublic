#include "Audio.h"

#include "_AudioEngine.h"

void Audio::SetMasterGain(float masterGain)
{
	_AudioEngine::GetInstance()->SetMasterGain(masterGain);
}

float Audio::GetMasterGain()
{
	return _AudioEngine::GetInstance()->GetMasterGain();
}

void Audio::SetMinAttenuationDistance(float distance)
{
	_AudioEngine::GetInstance()->SetMinAttenuationDistance(distance);
}

float Audio::GetMinAttenuationDistance()
{
	return _AudioEngine::GetInstance()->GetMinAttenuationDistance();
}

void Audio::SetMaxAttenuationDistance(float distance)
{
	_AudioEngine::GetInstance()->SetMaxAttenuationDistance(distance);
}

float Audio::GetMaxAttenuationDistance()
{
	return _AudioEngine::GetInstance()->GetMaxAttenuationDistance();
}

void Audio::SetMinPanningDistance(float distance)
{
	_AudioEngine::GetInstance()->SetMinPanningDistance(distance);
}

float Audio::GetMinPanningDistance()
{
	return _AudioEngine::GetInstance()->GetMinPanningDistance();
}

void Audio::SetMaxPanningDistance(float distance)
{
	_AudioEngine::GetInstance()->SetMaxPanningDistance(distance);
}

float Audio::GetMaxPanningDistance()
{
	return _AudioEngine::GetInstance()->GetMaxPanningDistance();
}
