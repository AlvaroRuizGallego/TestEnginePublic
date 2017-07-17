#ifndef COLLISION_BENCHMARK_SCENE_H
#define COLLISION_BENCHMARK_SCENE_H

#include "Scene.h"
#include "AudioSource.h"
#include "Rigidbody.h"
#include "SpriteSheet.h"
#include "Collider.h"

#include <glm.hpp>

#include "CollisionData.h"


class CollisionBenchmarkScene : public Scene
{
	private:

		//Camera
		Camera* _camera;

		//GameObjects

		int _nBoxes = 0;
		vec2 mousePosition;

		std::vector<RaycastHit> raycastHits;



		virtual void onInitialize() override;
		virtual void onTerminate() override;

		virtual void update(double deltaTime) override;
		virtual void fixedUpdate(float fixedTimestep) override;


		virtual void onButtonDown(int button) override;
		virtual void onButtonUp(int button) override;
		virtual void onMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition) override;
		virtual void onMouseMove(glm::vec2 position, glm::vec2 worldPosition) override;



	public:

		CollisionBenchmarkScene();
		~CollisionBenchmarkScene();
};

REGISTER_SCENE(CollisionBenchmarkScene);

#endif