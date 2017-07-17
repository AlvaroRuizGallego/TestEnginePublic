#include "CollisionBenchmarkScene.h"

#include "SpriteSheet.h"
#include "InputBinds.h"

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "AnimatedSpriteRenderer.h"
#include "Rigidbody.h"

#include "Drawer.h"
#include "_PhysicsEngine.h"
#include "CollisionQuerier.h"
#include "MaskManager.h"

#include "TextLabel.h"

#include "Random.h"

#include <vector>


CollisionBenchmarkScene::CollisionBenchmarkScene()
{
}

CollisionBenchmarkScene::~CollisionBenchmarkScene()
{
}

void CollisionBenchmarkScene::onInitialize()
{
	//Background
	GameObject * backgroundGameObject = CreateGameObject();
	SpriteRenderer* backgroundSpriteRenderer = backgroundGameObject->AddComponent(new SpriteRenderer("background.png"));
	backgroundGameObject->SetScale(GetReferenceWindowWidth() / backgroundSpriteRenderer->GetSpriteSize().x, GetReferenceWindowHeight() / backgroundSpriteRenderer->GetSpriteSize().y);

	//Platform
	GameObject* platformGameObject = CreateGameObject();
	platformGameObject->AddComponent(new SpriteRenderer("platform.png"));

	platformGameObject->AddComponent(new Collider({ { -312, 8 },{ 312, 8 },{ 312, -8 },{ -312, -8 } }, true, false, PhysicMaterial(1, 1)));

	platformGameObject->SetScale(2, 2);
	platformGameObject->SetPosition( 0, -200);
	platformGameObject->SetDepth(-1);


	//Platform 2
	GameObject* platformGameObject2 = CreateGameObject();
	platformGameObject2->AddComponent(new SpriteRenderer("platform.png"));

	platformGameObject2->AddComponent(new Collider({ { -312, 8 },{ 312, 8 },{ 312, -8 },{ -312, -8 } }, true, false, PhysicMaterial(1, 1)));

	platformGameObject2->SetScale(2, 2);
	platformGameObject2->SetPosition(600, 300);
	platformGameObject2->SetDepth(-1);
	platformGameObject2->SetOrientation(radians(-30.0f));

	//Platform 3
	GameObject* platformGameObject3 = CreateGameObject();
	platformGameObject3->AddComponent(new SpriteRenderer("platform.png"));

	platformGameObject3->AddComponent(new Collider({ { -312, 8 },{ 312, 8 },{ 312, -8 },{ -312, -8 } }, true, false, PhysicMaterial(1, 1)));

	platformGameObject3->SetScale(2, 2);
	platformGameObject3->SetPosition(-600, 500);
	platformGameObject3->SetDepth(-1);
	platformGameObject3->SetOrientation(radians(30.0f));


	//Rock
	GameObject* rockGameObject = CreateGameObject();
	rockGameObject->AddComponent(new SpriteRenderer("rock1.png"));


	Collider* colliderA = new Collider({ { -40, -33 },{ -10, 33 },{ 23, 20 },{ 10, -33 } }, true, false, PhysicMaterial(1, 1));
	Collider* colliderB = new Collider({ { 20, 11 },{ 40, -33 },{ 10, -33 } }, true, false, PhysicMaterial(1, 1));

	colliderA->tag = 10;
	colliderB->tag = 20;

	rockGameObject->AddComponent(colliderA);
	rockGameObject->AddComponent(colliderB);

	rockGameObject->SetScale(2, 2);
	rockGameObject->SetPosition( -200, -150);
	rockGameObject->SetDepth(-0.5);
}

void CollisionBenchmarkScene::onTerminate()
{
}

void CollisionBenchmarkScene::update(double deltaTime)
{
	/*for (RaycastHit raycastHit : raycastHits)
	{
		Drawer::GetInstance()->DrawCross(raycastHit.contactPoint, 20);
	}

	Drawer::GetInstance()->DrawLine(vec2(), mousePosition);*/
}

void CollisionBenchmarkScene::fixedUpdate(float fixedTimestep)
{

}

void CollisionBenchmarkScene::onButtonDown(int button)
{
	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("ForcesTestScene");
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("CollisionTestScene2");
	}

	if (button == INPUT_KEY_R)
	{
		ChangeToScene("CollisionBenchmarkScene");
	}
}

void CollisionBenchmarkScene::onButtonUp(int button)
{
}

void CollisionBenchmarkScene::onMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition)
{
	if (button == INPUT_MOUSE_BUTTON_LEFT)
	{
		int nBoxesInstanced = 1;
		for (int i = 0; i < nBoxesInstanced; i++)
		{
			GameObject* boxGameObject = CreateGameObject();
			boxGameObject->AddComponent(new SpriteRenderer("box.png"));

			boxGameObject->AddComponent(new Collider({ { -13, 10 },{ 13, 12 },{ 12, -12 },{ -10, -12 } }, true, false, PhysicMaterial(0, 1)));
			
			boxGameObject->AddComponent(new Rigidbody(1, 1000, 1));

			boxGameObject->SetScale(3, 3);
			boxGameObject->SetPosition(worldPosition.x, worldPosition.y);
			boxGameObject->SetDepth(-0.1);
			boxGameObject->SetOrientation(radians(180.0f));

			_nBoxes++;
		}
	}
	else if (button == INPUT_MOUSE_BUTTON_RIGHT)
	{
		int nBoxesInstanced = 1;
		for (int i = 0; i < nBoxesInstanced; i++)
		{
			GameObject* boxGameObject = CreateGameObject();
			boxGameObject->AddComponent(new SpriteRenderer("box.png"))->SetColor({ 1, 0, 0, 1 });

			boxGameObject->AddComponent(new Collider({ { -13, 10 },{ 13, 12 },{ 12, -12 },{ -10, -12 } }, true, false, PhysicMaterial(1, 1)));

			boxGameObject->AddComponent(new Rigidbody(1, 1000, 1));

			boxGameObject->SetScale(3, 3);
			boxGameObject->SetPosition(worldPosition.x, worldPosition.y);
			boxGameObject->SetDepth(-0.1);
			boxGameObject->SetOrientation(radians(180.0f));

			_nBoxes++;
		}
	}
	else if (button == INPUT_MOUSE_BUTTON_MIDDLE)
	{
		int nBoxesInstanced = 1;
		for (int i = 0; i < nBoxesInstanced; i++)
		{
			GameObject* boxGameObject = CreateGameObject();
			boxGameObject->AddComponent(new SpriteRenderer("box.png"))->SetColor({ 0, 1, 1, 1 });

			boxGameObject->AddComponent(new Collider({ { -13, 10 },{ 13, 12 },{ 12, -12 },{ -10, -12 } }, true, false, PhysicMaterial(0, 0)));

			boxGameObject->AddComponent(new Rigidbody(1, 1000, 1));

			boxGameObject->SetScale(3, 3);
			boxGameObject->SetPosition(worldPosition.x, worldPosition.y);
			boxGameObject->SetDepth(-0.1);
			boxGameObject->SetOrientation(radians(180.0f));

			_nBoxes++;
		}
	}
}

void CollisionBenchmarkScene::onMouseMove(glm::vec2 position, glm::vec2 worldPosition)
{
	mousePosition = worldPosition;
}
