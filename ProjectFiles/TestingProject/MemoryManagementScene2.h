#ifndef MEMORY_MANAGEMENT_SCENE_2_H
#define MEMORY_MANAGEMENT_SCENE_2_H

#include "Scene.h"
#include "GameObject.h"

class MemoryManagementScene2 : public Scene
{
	private:
		GameObject* _gameObject;
		virtual void onInitialize() override;
		virtual void onTerminate() override;
		virtual void onButtonDown(int button);

};

REGISTER_SCENE(MemoryManagementScene2);

#endif

