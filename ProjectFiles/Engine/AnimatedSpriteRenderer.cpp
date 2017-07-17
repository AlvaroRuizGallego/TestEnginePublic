#include "AnimatedSpriteRenderer.h"
#include "AnimatedSpriteRenderer.h"
#include "AnimatedSpriteRenderer.h"
#include "AnimatedSpriteRenderer.h"
#include "AnimatedSpriteRenderer.h"
#include "AnimatedSpriteRenderer.h"
#include "AnimatedSpriteRenderer.h"

#include "_SpriteManager.h"
#include "SpriteRenderer.h"



void AnimatedSpriteRenderer::advanceFrames(int nFrames)
{
	if(_animationUVs.size() == 0) return;

	int nextFrameIndex = _frameIndex + nFrames;

	if (_looping)
	{
		while (nextFrameIndex > _animationUVs.size() - 1)
		{
			nextFrameIndex -= _animationUVs.size();
		}
	}
	else
	{
		nextFrameIndex = min(nextFrameIndex, (int)_animationUVs.size() - 1);
	}
	
	setFrameIndex(nextFrameIndex);

	_sprite->SetUVs(_animationUVs[_frameIndex]);
}

void AnimatedSpriteRenderer::setFrameIndex(int frameIndex)
{
	if (_frameIndex != frameIndex)
	{
		if (_frameIndex < frameIndex)
		{
			for (int i = _frameIndex + 1; i <= frameIndex; i++)
			{
				_frameActions[i]();
			}
		}
		else
		{
			for (int i = _frameIndex + 1; i < (int)_animationUVs.size(); i++)
			{
				_frameActions[i]();
			}

			for (int i = 0; i <= frameIndex; i++)
			{
				_frameActions[i]();
			}
		}


		_frameIndex = frameIndex;


	}
}


AnimatedSpriteRenderer::AnimatedSpriteRenderer(SpriteSheet * spriteSheet, std::string  baseName, float frameDuration, int startingFrame, bool looping, bool cameraSpace)
{
	_animationUVs = spriteSheet->GetAnimationUVs(baseName);

	if (_animationUVs.size() == 0)
	{
		printf("\nAnimatedSpriteRenderer::AnimatedSpriteRenderer -> SpriteSheet doesn't contain frames for that animation");
		_sprite = new _Sprite("");

		return;
	}

	_baseFrameDuration = _frameDuration = frameDuration <= 0 ? 1 : frameDuration;
	_startingIndex = glm::clamp(startingFrame, 0, (int)_animationUVs.size()-1);
	_frameIndex = -1;
	setFrameIndex(_startingIndex);

	string imagePath = spriteSheet->GetSpriteSheetImagePath();

	_sprite = new _Sprite(imagePath, cameraSpace, _animationUVs[_frameIndex]);
	_elapsedTime = 0;

	_playing = true;

	_looping = looping;
}

AnimatedSpriteRenderer::~AnimatedSpriteRenderer()
{
}

void AnimatedSpriteRenderer::Update(double deltaTime)
{
	SpriteRenderer::Update(deltaTime);

	if (!_playing) return;

	_elapsedTime += deltaTime;

	if (_elapsedTime > _frameDuration)
	{
		int framesToAdvance = int(_elapsedTime / _frameDuration);
		advanceFrames(framesToAdvance);
		_elapsedTime -= _frameDuration * framesToAdvance;
	}
}

int AnimatedSpriteRenderer::GetFrameCount()
{
	return _animationUVs.size();
}

void AnimatedSpriteRenderer::Play()
{
	if (!_playing)
	{
		_playing = true;
		_sprite->SetUVs(_animationUVs[_frameIndex]);
	}
}

void AnimatedSpriteRenderer::Stop()
{
	if (_playing)
	{
		_playing = false;
		_frameIndex = _startingIndex;
		_elapsedTime = 0;
	}
}

void AnimatedSpriteRenderer::Pause()
{
	_playing = false;
}

void AnimatedSpriteRenderer::Replay()
{
	Stop();
	Play();
}

void AnimatedSpriteRenderer::SetFrameDuration(float frameDuration)
{
	_frameDuration = frameDuration;
}

float AnimatedSpriteRenderer::GetFrameDuration()
{
	return _frameDuration;
}

float AnimatedSpriteRenderer::GetBaseFrameDuration()
{
	return _baseFrameDuration;
}

int AnimatedSpriteRenderer::AddFrameCallback(int frameIdex, Callback callback)
{
	return _frameActions[frameIdex].AddCallback(callback);
}

void AnimatedSpriteRenderer::RemoveFrameCallback(int frameIdex, int callbackKey)
{
	_frameActions[frameIdex].RemoveCallback(callbackKey);
}





