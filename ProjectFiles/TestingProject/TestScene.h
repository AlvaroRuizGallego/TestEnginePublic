#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "Scene.h"
#include "SceneManager.h"

class TestScene: public Scene
{
	protected:
		virtual void onInitialize() override;
};

REGISTER_SCENE(TestScene);

#endif

