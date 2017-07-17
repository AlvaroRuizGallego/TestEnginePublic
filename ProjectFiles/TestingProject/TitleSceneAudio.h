#ifndef TITLE_SCENE_AUDIO_H
#define TITLE_SCENE_AUDIO_H

#include "Scene.h"

class TitleSceneAudio : public Scene
{
	public:
		void onInitialize() override;
		void onButtonDown(int button) override;
};

REGISTER_SCENE(TitleSceneAudio)

#endif
