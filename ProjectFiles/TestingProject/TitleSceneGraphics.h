#ifndef TITLE_SCENE_GRAPHICS_H
#define TITLE_SCENE_GRAPHICS_H

#include "Scene.h"

class TitleSceneGraphics : public Scene
{
	public:
		void onInitialize() override;
		void onButtonDown(int button) override;
};

REGISTER_SCENE(TitleSceneGraphics)

#endif
