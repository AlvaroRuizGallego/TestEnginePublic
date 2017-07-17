#include "_AudioLoader.h"


#include <fstream>

_AudioLoader* _AudioLoader::_instance = nullptr;

_AudioLoader::_AudioLoader()
{
}

_AudioLoader::~_AudioLoader()
{
}

_AudioLoader * _AudioLoader::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new _AudioLoader();
	}

	return _instance;
}

ALbyte * _AudioLoader::LoadWAV(std::string filename, int & format, int & size, int & frequency)
{
	auto convertToInt = [](char* buffer, int lenght)
	{
		int i = 0;
		memcpy(&i, buffer, lenght);
		return i;
	};

	char buffer[4];
	std::ifstream in(filename, std::ios::binary);

	in.read(buffer, 4); //ChunkID:			Contains the letters "RIFF" in ASCII form
	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		printf("\nWARNING: _AudioLoader::LoadWAV -> File is not a valid WAVE file 1");
		return NULL;
	}

	in.read(buffer, 4); //ChunkSize:		This is the size of the  entire file in bytes minus 8 bytes
	in.read(buffer, 4); //Format:			Contains the letters "WAVE"
	if (strncmp(buffer, "WAVE", 4) != 0)
	{
		printf("\nWARNING: _AudioLoader::LoadWAV -> File is not a valid WAVE file 2");
		return NULL;
	}

	in.read(buffer, 4); //Subchunk1ID:		Contains the letters "fmt "
	in.read(buffer, 4); //Subchunk1Size:	16 for PCM. This is the size of the rest of the Subchunk which follows this number.
	int extraParamSize = convertToInt(buffer, 4) - 16;

	in.read(buffer, 2); //AudioFormat		Values other than 1 indicate some form of compression.

	in.read(buffer, 2); //NumChannels		Mono = 1, Stereo = 2, etc.
	int nChannels = convertToInt(buffer, 2);

	in.read(buffer, 4); //SampleRate		8000, 44100, etc. Frequency
	frequency = convertToInt(buffer, 4);

	in.read(buffer, 4); //ByteRate			SampleRate(frequency) * NumChannels * BitsPerSample/8
	in.read(buffer, 2); //BlockAlign        NumChannels * BitsPerSample/8

	in.read(buffer, 2); //BitsPerSample		8 bits = 8, 16 bits = 16, etc.
	int bps = convertToInt(buffer, 2);

	if (nChannels == 1)
	{
		if (bps == 8)
		{
			format = AL_FORMAT_MONO8;
		}
		else if (bps == 16)
		{
			format = AL_FORMAT_MONO16;
		}
		else
		{
			printf("\nWARNING: _AudioLoader::LoadWAV-> Incorrect BPS value");
			return NULL;
		}
	}
	else if (nChannels == 2)
	{
		if (bps == 8)
		{
			format = AL_FORMAT_STEREO8;
		}
		else if (bps == 16)
		{
			format = AL_FORMAT_STEREO16;
		}
		else
		{
			printf("\nWARNING: _AudioLoader::LoadWAV-> Incorrect BPS value");
			return NULL;
		}
	}
	else
	{
		printf("\nWARNING: _AudioLoader::LoadWAV-> Incorrect number of channels");
		return NULL;
	}

	if (extraParamSize > 0)
	{
		in.read(buffer, extraParamSize); //Extra parameters
	}

	in.read(buffer, 4); //Subchunk2ID		Contains the letters "data"

	in.read(buffer, 4); //Subchunk2Size		NumSamples * NumChannels * BitsPerSample/8. This is the number of bytes in the data.
	size = convertToInt(buffer, 4);

	ALbyte* data = new ALbyte[size];
	in.read(data, size); //Data				The actual sound data.

	float duration = (float)size / (float)(frequency * nChannels * bps / 8);

	printf("\n\nWAV file loaded correctly\n-----------------------------------");
	printf("\nName: %s", filename);
	printf("\nSize: %i", size);
	printf("\nFrequency: %i", frequency);
	printf("\nNumber of channels: %i", nChannels);
	printf("\nBits Per Second: %i", bps);
	printf("\nDuration: %g", duration);
	printf("\n-----------------------------------");

	return (ALbyte*)data;
}
