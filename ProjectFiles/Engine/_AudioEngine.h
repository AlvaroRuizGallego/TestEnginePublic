#ifndef _AUDIO_ENGINE_H
#define _AUDIO_ENGINE_H


#include "glm.hpp"

#include <string>
#include <map>


struct _Source3dProperties
{
	bool is3d;
	glm::vec2 position;
	float gain;
	float attenuationFactor;
};

class _AudioEngine
{
	private:

		static _AudioEngine* _instance;

		static const int N_SOURCES = 32;
		static const int N_BUFFERS = 32;


		unsigned int _sources[N_SOURCES];
		unsigned int _buffers[N_BUFFERS];
		void* _device;
		void *_context;

		std::map<std::string, unsigned int> _audioMap; //Key: Filename, Value: BufferID
		int _nBuffersUsed;

		float _minAttenuationDistance;
		float _maxAttenuationDistance; 
		float _minPanningDistance;
		float _maxPanningDistance;

		int _listenerZPosition;

		std::map<unsigned int, _Source3dProperties> _source3dPropertiesMap;

		_AudioEngine();
		void checkForErrors(std::string  debugText);
		void initializeAL();
		void initializeListener();
		void createSources();
		void createBuffers();

		int getFreeSource();
		void setSource3dProperties(unsigned int sourceID, _Source3dProperties source3dProperties);

		void clearSourceProperties(unsigned int sourceID);
		void updateListenerZPosition();

	public:
		~_AudioEngine();

		static _AudioEngine* GetInstance();

		void SetParameters(float masterGain, float minAttenuationDistance, float maxAttenuationDistance, float minPanningDistance, float maxPanningDistance);
		void Clear();
		int LoadAudio(const std::string filename);
		int PlayAudio(unsigned int bufferID, glm::vec2 position, bool looping, bool is3d, float gain, float pitch, float attenuationFactor);
		void PlayAudioFromSource(unsigned int sourceID);
		void StopAudio(unsigned int sourceID);
		void PauseAudio(unsigned int sourceID);

		void SetAudioProperties(unsigned int sourceID, unsigned int bufferID, glm::vec2 position, bool looping, bool is3d, float gain, float pitch, float attenuationFactor);

		float GetPlaybackTime(unsigned int sourceID);
		void SetPlaybackTime(unsigned int sourceID, float playbackTime);

		void SetMasterGain(float gain);
		float GetMasterGain();
		void SetListenerProperties(glm::vec2 position, float orientation);

		void SetDistances(float minAttenuationDistance, float maxAttenuationDistance, float minPanningDistance, float maxPanningDistance);
		void SetMinAttenuationDistance(float distance);
		float GetMinAttenuationDistance();
		void SetMaxAttenuationDistance(float distance);
		float GetMaxAttenuationDistance();
		void SetMinPanningDistance(float distance);
		float GetMinPanningDistance();
		void SetMaxPanningDistance(float distance);
		float GetMaxPanningDistance();


		bool IsPlaying(unsigned int sourceID);
		bool IsPaused(unsigned int sourceID);
};



#endif