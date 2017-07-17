#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include "Component.h"


class __declspec(dllexport) AudioSource : public Component
{
	private:
		int _bufferID = -1;
		int _sourceID = -1;
		
		std::string _audioFile;
		bool _looping;
		bool _is3d;
		float _gain;
		float _pitch;
		float _attenuationFactor;

		float _baseGain;
		float _fadeSpeed;
		enum FadeState { NONE, FADING_IN_PLAY, FADING_OUT_STOP, FADING_OUT_PAUSE } _fadeState;

		void updateAudioProperties();

		void play();
		void stop();
		void pause();


	public:
		AudioSource(std::string audioFile, bool looping, bool is3d = false, float gain = 1.0, float pitch = 1.0, float attenuationFactor = 1.0);
		~AudioSource();

		void Update(double deltaTime) override;

		void Play(float fadeTime = 0, float offset = 0);
		void Replay(float fadeTime = 0, float offset = 0);
		void Stop(float fadeTime = 0);
		void Pause(float fadeTime = 0);

		void SetAudioFile(std::string audioFile);
		std::string GetAudioFile();

		void SetLooping(bool looping);
		bool GetLooping();

		void SetIs3d(bool is3d);
		bool GetIs3d();

		void SetPitch(float pitch);
		float GetPitch();

		void SetGain(float gain);
		float GetGain();

		float GetPlaybackTime();
		void SetPlaybackTime(float playbackTime);

		bool IsPlaying();
};


#endif
