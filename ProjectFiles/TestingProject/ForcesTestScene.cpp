#include "ForcesTestScene.h"

#include "Collider.h"
#include "ColliderDrawer.h"
#include "Random.h"

void ForcesTestScene::onInitialize()
{
	//Negative platform
	std::vector<vec2> vertices;
	float radius = 70;
	int colliderSides = 30;

	float correctionOrientation = radians(180 - 360.0 / colliderSides) * 0.5f;

	for (int i = 0; i < colliderSides; i++)
	{
		float y = sin(radians(360.0 / colliderSides* i) - correctionOrientation) * radius;
		float x = cos(radians(360.0 / colliderSides* i) - correctionOrientation) * radius;

		vertices.push_back({ x,y });
	}

	_bluePlatform = CreateGameObject();
	_bluePlatform->AddComponent(new Collider(vertices, true, false, PhysicMaterial(1, 1)));
	_bluePlatform->AddComponent(new ColliderDrawer(4.0, {0,1,1,1}));
	_bluePlatform->SetPosition(300, 0);

	//Positive platform
	_redPlatform = CreateGameObject();
	_redPlatform->AddComponent(new Collider(vertices, true, false, PhysicMaterial(1, 1)));
	_redPlatform->AddComponent(new ColliderDrawer(4.0, { 1,0,0,1 }));
	_redPlatform->SetPosition(-300, 0);

	//Parent
	_platformParent = CreateGameObject();
	_platformParent->AddChildGameObject(_bluePlatform);
	_platformParent->AddChildGameObject(_redPlatform);
	//_platformParent->SetPositionY(-150);
}

void ForcesTestScene::onTerminate()
{
	_redCircles.clear();
	_blueCircles.clear();
}

void ForcesTestScene::update(double deltaTime)
{
	for (Rigidbody* rb : _redCircles)
	{
		vec2 redDirection = (vec2)_redPlatform->GetPosition(false) -  (rb->GetGameObject() != nullptr ? (vec2)rb->GetGameObject()->GetPosition() : vec2());
		rb->ApplyForce((vec2)normalize(redDirection) * 75000.0f * (float)deltaTime);
	}

	for (Rigidbody* rb : _blueCircles)
	{
		vec2 blueDirection = (vec2)_bluePlatform->GetPosition(false) - (rb->GetGameObject() != nullptr ? (vec2)rb->GetGameObject()->GetPosition() : vec2());
		rb->ApplyForce((vec2)normalize(blueDirection) * 75000.0f * (float)deltaTime);
	}
}

void ForcesTestScene::onMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition)
{
	if (button == INPUT_MOUSE_BUTTON_LEFT)
	{
		instantiateCircle(worldPosition, false);
	}
	else if (button == INPUT_MOUSE_BUTTON_RIGHT)
	{
		instantiateCircle(worldPosition, true);
	}


}

GameObject* ForcesTestScene::instantiateCircle(vec2 position, bool blue)
{
	GameObject* gameObject = CreateGameObject();

	std::vector<vec2> vertices;
	float radius = 30;
	int colliderSides = 30;

	float correctionOrientation = radians(180 - 360.0 / colliderSides) * 0.5f;

	for (int i = 0; i < colliderSides; i++)
	{
		float y = sin(radians(360.0 / colliderSides* i) - correctionOrientation) * radius;
		float x = cos(radians(360.0 / colliderSides* i) - correctionOrientation) * radius;

		vertices.push_back({ x,y });
	}

	Collider* collider = new Collider(vertices, false, false, PhysicMaterial(0, 1));
	Rigidbody* rigidbody = new Rigidbody(1, 1500, 0);

	gameObject->AddComponent(collider);
	gameObject->AddComponent(rigidbody);

	vec4 color;
	if (blue)
	{
		color = { 0,1,1,1 };
		_blueCircles.push_back(rigidbody);
	}
	else
	{
		_redCircles.push_back(rigidbody);
		color = { 1,0,0,1 };
	}

	gameObject->AddComponent(new ColliderDrawer(4.0, color));

	gameObject->SetPosition(position.x, position.y);

	return gameObject;
}

void ForcesTestScene::onButtonDown(int button)
{
	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}

	if (button == INPUT_KEY_R)
	{
		ChangeToScene("ForcesTestScene");
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("TitleScene");
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("CollisionBenchmarkScene");
	}

	if (button == INPUT_KEY_C)
	{
		ColliderDrawer::drawContactPoints = !ColliderDrawer::drawContactPoints;
	}
}



