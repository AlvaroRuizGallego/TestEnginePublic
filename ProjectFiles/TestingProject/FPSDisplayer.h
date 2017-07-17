#ifndef FPS_DISPLAYER_H
#define FPS_DISPLAYER_H

#include "Component.h"
#include "TextLabel.h"

class FPSDisplayer: public Component
{
	private:
		TextLabel* _textLabel;
		double _elapsedTime;
		int _nFrames;
		float _updateTime;

	public:
		FPSDisplayer(float updateTime = 1);
		~FPSDisplayer();

		void OnInitialize() override;
		void Update(double deltaTime) override;
};

#endif
