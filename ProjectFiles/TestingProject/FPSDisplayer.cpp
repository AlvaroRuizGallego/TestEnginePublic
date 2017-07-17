#include "FPSDisplayer.h"

#include <string>

FPSDisplayer::FPSDisplayer(float updateTime)
{
	_updateTime = updateTime;
}


FPSDisplayer::~FPSDisplayer()
{
}

void FPSDisplayer::OnInitialize()
{
	_textLabel = _gameObject->GetComponent<TextLabel>();

	if (_textLabel == nullptr)
	{
		_textLabel = _gameObject->AddComponent(new TextLabel("FPS: ", "VCR_OSD_MONO.ttf", true, 0.5, 1.0, { 1, 1, 0, 1 }, TextLabel::H_LEFT, TextLabel::V_TOP));
	}

	_elapsedTime = 0;
	_nFrames = 0;
}

void FPSDisplayer::Update(double deltaTime)
{
	_elapsedTime += deltaTime;
	_nFrames++;

	if (_elapsedTime >= _updateTime)
	{
		float fps = 1/(_elapsedTime / (float)_nFrames);

		_textLabel->SetText("FPS:" + std::to_string((int)fps));

		_elapsedTime = 0;
		_nFrames = 0;
	}
}
