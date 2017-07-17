#include "TestScene.h"

#include "TestComponent.h"
#include "SpriteRenderer.h"

void TestScene::onInitialize()
{
	GameObject* gameObject = CreateGameObject();

	SpriteRenderer* sr = gameObject->AddComponent(new SpriteRenderer("0.png"));
	gameObject->SetScaleX(GetReferenceWindowWidth() / sr->GetSpriteSize().x);
	gameObject->SetScaleY(GetReferenceWindowHeight() / sr->GetSpriteSize().y);
}

