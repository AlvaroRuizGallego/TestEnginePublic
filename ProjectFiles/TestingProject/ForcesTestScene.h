#ifndef FORCES_TEST_SCENE_H
#define FORCES_TEST_SCENE_H

#include "Scene.h"
#include "Rigidbody.h"
#include "Collider.h"

class ForcesTestScene : public Scene
{
	private:
		vector<Rigidbody*> _redCircles;
		vector<Rigidbody*> _blueCircles;

		GameObject* _bluePlatform;
		GameObject* _redPlatform;
		GameObject* _platformParent;

		void onInitialize() override;
		void onTerminate() override;
		void onButtonDown(int button) override;
		void update(double deltaTime) override;
		void onMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition) override;

		GameObject* instantiateCircle(vec2 position, bool blue);
};

REGISTER_SCENE(ForcesTestScene);

#endif
