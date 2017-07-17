#ifndef SPRITE_ANIMATOR_H
#define SPRITE_ANIMATOR_H

#include "Component.h"

#include "SpriteSheet.h"
#include "SpriteRenderer.h"

#include "Action.h"

class __declspec(dllexport) AnimatedSpriteRenderer: public SpriteRenderer
{
	private:
		float _frameDuration;
		float _baseFrameDuration;
		int _frameIndex;
		int _startingIndex;
		double _elapsedTime;
		bool _looping;

		bool _playing;

		std::vector<Rectangle> _animationUVs;

		std::map<int, Action> _frameActions;

		void advanceFrames(int nFrames);
		void setFrameIndex(int frameIndex);

	public:
		AnimatedSpriteRenderer(SpriteSheet* spriteSheet, std::string  baseName, float frameDuration, int startingFrame = 0, bool looping = true, bool cameraSpace = false);
		~AnimatedSpriteRenderer();

		virtual void Update(double deltaTime) override;

		int GetFrameCount();

		void Play();
		void Stop();
		void Pause();
		void Replay();

		void SetFrameDuration(float frameDuration);
		float GetFrameDuration();
		float GetBaseFrameDuration();

		int AddFrameCallback(int frameIdex, Callback callback);
		void RemoveFrameCallback(int frameIdex, int callbackKey);
};

#endif
