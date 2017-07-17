#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include "Scene.h"

class TitleScene : public Scene
{
	public:
		void onInitialize() override;
		void onButtonDown(int button) override;
};

REGISTER_SCENE(TitleScene)

#endif
