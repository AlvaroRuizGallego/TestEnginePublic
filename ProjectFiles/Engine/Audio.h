#ifndef ENGINE_AUDIO_H
#define ENGINE_AUDIO_H


class __declspec(dllexport) Audio
{
	public:
		static void SetMasterGain(float masterGain);
		static float GetMasterGain();
		static void SetMinAttenuationDistance(float distance);
		static float GetMinAttenuationDistance();
		static void SetMaxAttenuationDistance(float distance);
		static float GetMaxAttenuationDistance();
		static void SetMinPanningDistance(float distance);
		static float GetMinPanningDistance();
		static void SetMaxPanningDistance(float distance);
		static float GetMaxPanningDistance();
};

#endif