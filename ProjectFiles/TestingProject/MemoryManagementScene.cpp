#include "MemoryManagementScene.h"

#include "SpriteRenderer.h"
#include "AudioSource.h"
#include "TextLabel.h"
#include "Random.h"
#include "Rigidbody.h"
#include "Collider.h"

#include <string>

void MemoryManagementScene::onInitialize()
{
	printf("\nMemoryManagementScene -> onInitialize");

	GameObject* cameraGameObject = CreateGameObject();
	cameraGameObject->AddComponent(new Camera());

	//addGameObject(CreateGameObject(););


	_gameObject = CreateGameObject();

	//Sprite
	//std::string spriteNames[] = { "background.png", "gooniesSameSizeCentered.png", "parakoopa.png", "background2.png" };
	//_gameObject->AddComponent(new SpriteRenderer("background2.png"));

	//Audio
	_gameObject->AddComponent(new AudioSource("athletic.wav", true));
	//_gameObject->GetComponent<AudioSource>()->Play();

	//Text
	_gameObject->AddComponent(new TextLabel("bbbbbbbbbbbbbb", "FreeSans.ttf", false, 1, 1, glm::vec4(1, 1, 1, 1), TextLabel::TextHorizontalAlignment::H_CENTER, TextLabel::TextVerticalAlignment::V_CENTER));

	//Physics
	_gameObject->AddComponent(new Rigidbody(1, 1000, 0));
	_gameObject->AddComponent(new Collider({ { -40, -33 },{ -10, 31 },{ 23, 18 },{ 10, -33 } }, true, false, PhysicMaterial(1, 0.8)));
}

void MemoryManagementScene::onTerminate()
{
}

void MemoryManagementScene::onButtonDown(int button)
{
	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("CollisionBenchmarkScene");
	}
}

