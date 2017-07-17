#ifndef MEMORY_MANAGEMENT_SCENE_H
#define MEMORY_MANAGEMENT_SCENE_H

#include "Scene.h"
#include "GameObject.h"

class MemoryManagementScene: public Scene
{
	private:
		GameObject* _gameObject;
		virtual void onInitialize() override;
		virtual void onTerminate() override;
		virtual void onButtonDown(int button);

};

REGISTER_SCENE(MemoryManagementScene);

#endif

