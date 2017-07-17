#include "CollisionTestScene2.h"

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
#include "ColliderDrawer.h"
#include "FPSDisplayer.h"

#include "Random.h"

#include "TextLabel.h"

#include "Drawer.h"

#include "TestComponent.h"

#include <vector>


CollisionTestScene2::CollisionTestScene2()
{
}

CollisionTestScene2::~CollisionTestScene2()
{
}

void CollisionTestScene2::onInitialize()
{
	//Camera
	GameObject* cameraGameObject = CreateGameObject();
	cameraGameObject->AddComponent(new Camera());

	//Platform
	_platformGameObject = CreateGameObject();

	_platformGameObject->AddComponent(new Collider({ { 1000, 500 },{ 1000, -500 },{ -1000, -500 },{ -1000, 500 } }, true, false, PhysicMaterial(1, 1)));
	_platformGameObject->AddComponent(new ColliderDrawer(4.0));

	_platformGameObject->SetPosition(0, -650);
	_platformGameObject->GetComponent<Collider>()->tag = 0;

	_nColliders++;

	//Platform 2
	GameObject* platformGameObject2 = CreateGameObject();
	platformGameObject2->AddComponent(new Collider({ { -312, 8 },{ 312, 8 },{ 312, -8 },{ -312, -8 } }, true, false, PhysicMaterial(1, 1)));
	platformGameObject2->AddComponent(new ColliderDrawer(4.0));

	platformGameObject2->SetScale(2, 2);
	platformGameObject2->SetPosition(600, 300);
	platformGameObject2->SetDepth(-1);
	platformGameObject2->SetOrientation(radians(-30.0f));

	//Platform 3
	GameObject* platformGameObject3 = CreateGameObject();

	platformGameObject3->AddComponent(new Collider({ { -312, 8 },{ 312, 8 },{ 312, -8 },{ -312, -8 } }, true, false, PhysicMaterial(1, 1)));
	platformGameObject3->AddComponent(new ColliderDrawer(4.0));

	platformGameObject3->SetScale(2, 2);
	platformGameObject3->SetPosition(-600, 500);
	platformGameObject3->SetDepth(-1);
	platformGameObject3->SetOrientation(radians(30.0f));


	//Sides
	GameObject* textGameObject = CreateGameObject();
	_sidesText =  textGameObject->AddComponent(new TextLabel("3", "VCR_OSD_MONO.ttf", true, 0.5, 1.0, vec4(1, 1, 1, 1), TextLabel::H_LEFT, TextLabel::V_TOP));

	textGameObject->SetPosition(-GetReferenceWindowWidth()*0.475, GetReferenceWindowHeight()*0.475);
}

void CollisionTestScene2::onTerminate()
{
	_nColliders = 0;
	_colliderSides = 3;
}

void CollisionTestScene2::update(double deltaTime)
{
	/*for (RaycastHit raycastHit : raycastHits)
	{
	Drawer::GetInstance()->DrawCross(raycastHit.contactPoint, 20, true, vec4(0, 1, 1, 1), 2);
	}*/

	//Drawer::GetInstance()->DrawLine(vec2(), mousePosition, vec4(1, 1, 0, 1), 2);
}

void CollisionTestScene2::fixedUpdate(float fixedTimestep)
{
}

void CollisionTestScene2::onButtonDown(int button)
{
	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}

	if (button == INPUT_KEY_W)
	{
		_colliderSides++;

		_sidesText->SetText(std::to_string(_colliderSides));
	}

	if (button == INPUT_KEY_S)
	{
		_colliderSides = max(_colliderSides - 1, 3);

		_sidesText->SetText(std::to_string(_colliderSides));
	}

	if (button == INPUT_KEY_C)
	{
		ColliderDrawer::drawContactPoints = !ColliderDrawer::drawContactPoints;
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("CollisionTestScene");
	}

	if (button == INPUT_KEY_R)
	{
		ChangeToScene("CollisionTestScene2");
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("CollisionBenchmarkScene");
	}

	if (button == INPUT_KEY_SPACE)
	{
		GameObject* circleGameObject = CreateGameObject();

		std::vector<vec2> vertices;
		float radius = 35;

		for (int i = 0; i < 25; i++)
		{
			float x = cos(radians(360.0 / 25 * i)) * radius;
			float y = sin(radians(360.0 / 25 * i)) * radius;

			vertices.push_back({ x,y });
		}

		instantiateColliderGameObject(new Collider(vertices, false, false, PhysicMaterial(1, 1)), new Rigidbody(1, 1500, 1), mousePosition, { 1,1,1,1 })->GetComponent<Rigidbody>()->ApplyLinearImpulse({ 2000, 0 });
	}
}

void CollisionTestScene2::onButtonUp(int button)
{

}

void CollisionTestScene2::onMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition)
{
	if (button == INPUT_MOUSE_BUTTON_LEFT)
	{
		std::vector<vec2> vertices;
		float radius = 50;

		float correctionOrientation = radians(180 - 360.0 / _colliderSides) * 0.5f;

		for (int i = 0; i < _colliderSides; i++)
		{
			float y = sin(radians(360.0 / _colliderSides* i) - correctionOrientation) * radius;
			float x = cos(radians(360.0 / _colliderSides* i) - correctionOrientation) * radius;

			vertices.push_back({ x,y });
		}

		instantiateColliderGameObject(new Collider(vertices, false, false, PhysicMaterial(0, 1)), new Rigidbody(1, 1500, 1), worldPosition, { 0,1,0,1 });
	}
	else if (button == INPUT_MOUSE_BUTTON_RIGHT)
	{
		std::vector<vec2> vertices;
		float radius = 50;

		float correctionOrientation = radians(180 - 360.0 / _colliderSides) * 0.5f;

		for (int i = 0; i < _colliderSides; i++)
		{
			float y = sin(radians(360.0 / _colliderSides* i) - correctionOrientation) * radius;
			float x = cos(radians(360.0 / _colliderSides* i) - correctionOrientation) * radius;

			vertices.push_back({ x,y });
		}

		instantiateColliderGameObject(new Collider(vertices, false, false, PhysicMaterial(1, 1)), new Rigidbody(10, 15000, 1), worldPosition, { 1,1,0,1 });
		//instantiateColliderGameObject(new Collider(vertices, false, false, PhysicMaterial(0, 1)), new Rigidbody(10, 15000, 1), worldPosition);
	}
	else if (button == INPUT_MOUSE_BUTTON_MIDDLE)
	{
		/*raycastHits = CollisionQuerier::GetInstance()->RaycastAll(vec2(), worldPosition);

		float impulseMagnitude = 750;

		for (RaycastHit hit : raycastHits)
		{
		Rigidbody* rb = hit.collider->GetGameObject()->GetComponent<Rigidbody>();

		if (rb != nullptr)
		{
		rb->ApplyImpulseAtPoint(normalize(worldPosition) * impulseMagnitude, worldPosition);
		}
		}*/

		std::vector<vec2> vertices;
		float radius = 50;

		float correctionOrientation = radians(180 - 360.0 / _colliderSides) * 0.5f;

		for (int i = 0; i < _colliderSides; i++)
		{
			float y = sin(radians(360.0 / _colliderSides* i) - correctionOrientation) * radius;
			float x = cos(radians(360.0 / _colliderSides* i) - correctionOrientation) * radius;

			vertices.push_back({ x,y });
		}

		instantiateColliderGameObject(new Collider(vertices, false, false, PhysicMaterial(0, 0)), new Rigidbody(10, 15000, 1), worldPosition, { 0,1,1,1 });
	}
}


void CollisionTestScene2::onMouseMove(glm::vec2 position, glm::vec2 worldPosition)
{
	mousePosition = worldPosition;
}

GameObject* CollisionTestScene2::instantiateColliderGameObject(Collider * collider, Rigidbody* rigidbody, vec2 position, vec4 color)
{
	GameObject* gameObject = CreateGameObject();

	gameObject->AddComponent(collider);
	gameObject->AddComponent(rigidbody);
	//gameObject->AddComponent(new TextLabel(std::to_string(_nColliders + 1), "FreeSans.ttf", false, 0.1, 1, glm::vec4(1, 1, 1, 1), TextLabel::H_CENTER, TextLabel::V_CENTER));

	gameObject->AddComponent(new ColliderDrawer(4.0, color));

	gameObject->GetComponent<Collider>()->tag = _nColliders;

	gameObject->SetPosition(position.x, position.y);

	_nColliders++;

	return gameObject;
}
