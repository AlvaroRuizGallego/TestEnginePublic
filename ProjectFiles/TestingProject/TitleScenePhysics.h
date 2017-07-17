#ifndef TITLE_SCENE_PHYSICS_H
#define TITLE_SCENE_PHYSICS_H

#include "Scene.h"

class TitleScenePhysics : public Scene
{
	public:
		void onInitialize() override;
		void onButtonDown(int button) override;
};

REGISTER_SCENE(TitleScenePhysics)

#endif
