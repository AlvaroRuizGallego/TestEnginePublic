#ifndef SIMPLEST_SCENE_H
#define SIMPLEST_SCENE_H

#include "Scene.h"
class SimplestScene : public Scene
{	
	private:
		void onInitialize() override;


};

REGISTER_SCENE(SimplestScene)
#endif

