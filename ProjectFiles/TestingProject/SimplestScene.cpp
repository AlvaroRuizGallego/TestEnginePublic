#include "SimplestScene.h"
#include "SpriteRenderer.h"
#include "Random.h"

void SimplestScene::onInitialize()
{
	for (int i = 0; i < 100; ++i)
	{
		GameObject* character = CreateGameObject();
		character->AddComponent(new SpriteRenderer("character.png"));
		character->SetPosition(Random::Range(-GetReferenceWindowWidth()* 0.5f, GetReferenceWindowWidth()*0.5f), Random::Range(-GetReferenceWindowHeight()*0.5f, GetReferenceWindowHeight()*0.5f));
		character->SetScale(0.5, 0.5);
	}
	//GameObject* character = CreateGameObject();
	//character->AddComponent(new SpriteRenderer("character.png"));
}







