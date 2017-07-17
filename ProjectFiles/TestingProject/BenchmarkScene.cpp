#include "BenchmarkScene.h"

#include "SpriteSheet.h"
#include "InputBinds.h"

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "AnimatedSpriteRenderer.h"
#include "Rigidbody.h"

#include "TestStateMachine.h"
#include "AudioListener.h"

#include "SpriteSheetManager.h"

#include "FPSDisplayer.h"


BenchmarkScene::BenchmarkScene()
{
}

BenchmarkScene::~BenchmarkScene()
{
}

void BenchmarkScene::onInitialize()
{
	//Spritesheet
	_spriteSheet = SpriteSheetManager::GetInstance()->GetSpriteSheet("gooniesSameSizeCentered.json");

	//Goonies
	for (int i = 0; i < _nGoonies; ++i)
	{
		GameObject* goonie = CreateGameObject();
		goonie->AddComponent(new AnimatedSpriteRenderer(_spriteSheet, "goonie_", 0.1f));
		goonie->AddComponent(new Rigidbody(1, 1, false));

		_goonies.push_back(goonie);
		instanceGoonie(goonie);
	}

	//FPS Displayer
	GameObject* fpsDisplayer = CreateGameObject();
	fpsDisplayer->AddComponent(new FPSDisplayer());
	fpsDisplayer->SetPosition(-GetReferenceWindowWidth()*0.475, GetReferenceWindowHeight()*0.475);
	fpsDisplayer->SetDepth(-10);
}

void BenchmarkScene::onTerminate()
{
	_goonies.clear();
}

void BenchmarkScene::update(double deltaTime)
{
	for (GameObject* goonie : _goonies)
	{
		if (goonie->GetPosition().x * goonie->GetComponent<AnimatedSpriteRenderer>()->GetParallax() + goonie->GetComponent<SpriteRenderer>()->GetSpriteSize().x * 0.5 * goonie->GetScale().x < -GetReferenceWindowWidth()*0.5)
		{
			instanceGoonie(goonie);
		}
	}
}

void BenchmarkScene::fixedUpdate(float fixedTimestep)
{
}


void BenchmarkScene::onButtonDown(int button)
{
	static bool transparent = false;
	printf("\n(%i) DOWN\n", button);

	if (button == INPUT_KEY_ESCAPE)
	{
		TerminateGame();
	}

	if (button == INPUT_KEY_R)
	{
		ChangeToScene("BenchmarkScene");
	}

	if (button == INPUT_KEY_ENTER)
	{
		ChangeToScene("TextTestScene");
	}

	if (button == INPUT_KEY_BACKSPACE)
	{
		ChangeToScene("TransparencyTestScene");
	}

	if (button == INPUT_KEY_SPACE)
	{
		transparent = !transparent;

		for (GameObject* goonie : _goonies)
		{
			if (transparent)
			{
				goonie->GetComponent<SpriteRenderer>()->SetColor({ 1, 1, 1, 0.1 });
			}
			else
			{
				goonie->GetComponent<SpriteRenderer>()->SetColor({ 1, 1, 1, 1 });
			}
		}
	}

}

void BenchmarkScene::instanceGoonie(GameObject * goonie)
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float scale = _goonieMinScale + r * (_goonieMaxScale - _goonieMinScale);
	goonie->SetScale(scale, scale);
	goonie->GetComponent<Rigidbody>()->SetLinearVelocityX(_goonieBaseSpeed);

	goonie->SetPositionX((GetReferenceWindowWidth()*0.5 + goonie->GetComponent<SpriteRenderer>()->GetSpriteSize().x * 0.5 * goonie->GetScale().x) / scale);

	r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	goonie->SetPositionY((-GetReferenceWindowHeight()*0.5 + GetReferenceWindowHeight()* r) / scale);
	goonie->SetDepth(-1 * goonie->GetScale().x);

	goonie->GetComponent<AnimatedSpriteRenderer>()->SetParallax(scale);
}
