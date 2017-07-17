#ifndef TRANSPARENCY_TEST_SCENE_H
#define TRANSPARENCY_TEST_SCENE_H

#include "Scene.h"
#include "SpriteRenderer.h"

#include <vector>

class TransparencyTestScene: public Scene
{
	private:
		void onInitialize() override;

		std::vector<SpriteRenderer*> _spriteRenderers; //TEST

		void onButtonDown(int button) override;

	public:
		TransparencyTestScene();
		~TransparencyTestScene();


};

REGISTER_SCENE(TransparencyTestScene);

#endif