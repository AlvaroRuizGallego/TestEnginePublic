#ifndef _AUDIO_LOADER_H
#define _AUDIO_LOADER_H

#include "al.h"

#include <string>

class _AudioLoader
{
private:
	static _AudioLoader* _instance;

	_AudioLoader();
	~_AudioLoader();


public:

	static _AudioLoader* GetInstance();

	ALbyte * LoadWAV(std::string filename, int &format, int &size, int &frequency);
};

#endif

