#include "TransparencyTestScene.h"

#include "SpriteRenderer.h"
#include "Random.h"
#include "Camera.h"
#include "CameraManualController.h"
#include "TextLabel.h"
#include "Rigidbody.h"

void TransparencyTestScene::onInitialize()
{
	//Camera
	GameObject* cameraGameObject = CreateGameObject();
	Camera* camera = cameraGameObject->AddComponent(new Camera());
	cameraGameObject->AddComponent(new CameraManualController());

	//camera->SetVignette("vignette.png");
	//camera->SetVignetteColor({ 1,1,1, 0.2});

	//Background
	GameObject* background = CreateGameObject();
	background->AddComponent(new SpriteRenderer("forest.jpg"))->SetParallax(0);
	background->SetDepth(1);

	//Circles
	int nCircles = 100;
	for (int i = 0; i < nCircles; i++)
	{
		float parallax = Random::Range(0.5f, 2.0f);

		//GameObject
		GameObject* gameObject = CreateGameObject();
		gameObject->SetPosition(Random::Range(-GetReferenceWindowWidth(), GetReferenceWindowWidth()), Random::Range(-GetReferenceWindowHeight(), GetReferenceWindowHeight()));
		gameObject->SetScale(parallax*0.5, parallax*0.5);
		gameObject->SetDepth(-parallax);

		//Sprite renderer
		SpriteRenderer* spriteRenderer = gameObject->AddComponent(new SpriteRenderer("circle.png"));
		spriteRenderer->SetParallax(parallax);
		spriteRenderer->SetColor({ Random::Range(0, 1),  Random::Range(0, 1),  Random::Range(0, 1), Random::Range(0.5f, 0.75f) });

		_spriteRenderers.push_back(spriteRenderer);
	}
}

void TransparencyTestScene::onButtonDown(int button)
{
	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}
	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("BenchmarkScene");
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("TitleSceneGraphics");
	}
}

TransparencyTestScene::TransparencyTestScene()
{
}


TransparencyTestScene::~TransparencyTestScene()
{
}
