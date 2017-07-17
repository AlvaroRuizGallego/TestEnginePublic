#ifndef COLLISION_TEST_SCENE_H
#define COLLISION_TEST_SCENE_H

#include "Scene.h"
#include "AudioSource.h"
#include "Rigidbody.h"
#include "SpriteSheet.h"
#include "Collider.h"

#include <glm.hpp>

#include "CollisionData.h"
#include "TextLabel.h"


class CollisionTestScene : public Scene
{
	private:

		//Camera
		Camera* _camera;

		//GameObjects
		GameObject* _platformGameObject;


		bool _movingBox = false;

		int _nColliders = 0;
		vec2 mousePosition;

		std::vector<RaycastHit> raycastHits;

		int _colliderSides = 3;

		TextLabel* _sidesText;


		virtual void onInitialize() override;
		virtual void onTerminate() override;

		virtual void update(double deltaTime) override;
		virtual void fixedUpdate(float fixedTimestep) override;


		virtual void onButtonDown(int button) override;
		virtual void onButtonUp(int button) override;
		virtual void onMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition) override;
		virtual void onMouseMove(glm::vec2 position, glm::vec2 worldPosition) override;

		GameObject* instantiateColliderGameObject(Collider* collider, Rigidbody* rigidbody, vec2 position);

	public:

		CollisionTestScene();
		~CollisionTestScene();
};

REGISTER_SCENE(CollisionTestScene);

#endif